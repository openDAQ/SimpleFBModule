#include <example_module/example_fb.h>
#include <example_module/dispatch.h>
#include <opendaq/event_packet_params.h>

BEGIN_NAMESPACE_EXAMPLE_MODULE
    ExampleFBImpl::ExampleFBImpl(const ContextPtr& ctx, const ComponentPtr& parent, const StringPtr& localId)
    : FunctionBlock(CreateType(), ctx, parent, localId)
{
    initComponentStatus();
    createInputPorts();
    createSignals();
    initProperties();
}

void ExampleFBImpl::initProperties()
{
    const auto scaleProp = FloatProperty("Scale", 1.0);
    objPtr.addProperty(scaleProp);
    objPtr.getOnPropertyValueWrite("Scale") +=
        [this](PropertyObjectPtr& obj, PropertyValueEventArgsPtr& args) { propertyChanged(true); };

    const auto offsetProp = FloatProperty("Offset", 0.0);
    objPtr.addProperty(offsetProp);
    objPtr.getOnPropertyValueWrite("Offset") +=
        [this](PropertyObjectPtr& obj, PropertyValueEventArgsPtr& args) { propertyChanged(true); };

    const auto useCustomOutputRangeProp = BoolProperty("UseCustomOutputRange", False);
    objPtr.addProperty(useCustomOutputRangeProp);
    objPtr.getOnPropertyValueWrite("UseCustomOutputRange") +=
        [this](PropertyObjectPtr& obj, PropertyValueEventArgsPtr& args) { propertyChanged(true); };

    const auto customHighValueProp = FloatProperty("OutputHighValue", 10.0, EvalValue("$UseCustomOutputRange"));
    objPtr.addProperty(customHighValueProp);
    objPtr.getOnPropertyValueWrite("OutputHighValue") +=
        [this](PropertyObjectPtr& obj, PropertyValueEventArgsPtr& args) { propertyChanged(true); };

    const auto customLowValueProp = FloatProperty("OutputLowValue", -10.0, EvalValue("$UseCustomOutputRange"));
    objPtr.addProperty(customLowValueProp);
    objPtr.getOnPropertyValueWrite("OutputLowValue") +=
        [this](PropertyObjectPtr& obj, PropertyValueEventArgsPtr& args) { propertyChanged(true); };

    const auto outputNameProp = StringProperty("OutputName", "");
    objPtr.addProperty(outputNameProp);
    objPtr.getOnPropertyValueWrite("OutputName") +=
        [this](PropertyObjectPtr& obj, PropertyValueEventArgsPtr& args) { propertyChanged(true); };

    const auto outputUnitProp = StringProperty("OutputUnit", "");
    objPtr.addProperty(outputUnitProp);
    objPtr.getOnPropertyValueWrite("OutputUnit") +=
        [this](PropertyObjectPtr& obj, PropertyValueEventArgsPtr& args) { propertyChanged(true); };

    readProperties();
}

void ExampleFBImpl::propertyChanged(bool configure)
{
    readProperties();
    if (configure)
        this->configure();
}

void ExampleFBImpl::readProperties()
{
    scale = objPtr.getPropertyValue("Scale");
    offset = objPtr.getPropertyValue("Offset");
    useCustomOutputRange = objPtr.getPropertyValue("UseCustomOutputRange");
    outputHighValue = objPtr.getPropertyValue("OutputHighValue");
    outputLowValue = objPtr.getPropertyValue("OutputLowValue");
    outputUnit = static_cast<std::string>(objPtr.getPropertyValue("OutputUnit"));
    outputName = static_cast<std::string>(objPtr.getPropertyValue("OutputName"));
}

FunctionBlockTypePtr ExampleFBImpl::CreateType()
{
    return FunctionBlockType("ExampleScalingModule", "Scaling", "Signal scaling");
}

void ExampleFBImpl::processSignalDescriptorChanged(const DataDescriptorPtr& dataDescriptor,
                                                   const DataDescriptorPtr& domainDescriptor)
{
    if (dataDescriptor.assigned())
        this->inputDataDescriptor = dataDescriptor;
    if (domainDescriptor.assigned())
        this->inputDomainDataDescriptor = domainDescriptor;

    configure();
}

void ExampleFBImpl::configure()
{
    try
    {
        if (!inputDomainDataDescriptor.assigned() || inputDomainDataDescriptor == NullDataDescriptor())
        {
            throw std::runtime_error("No domain input");
        }

        if (!inputDataDescriptor.assigned() || inputDataDescriptor == NullDataDescriptor())
        {
            throw std::runtime_error("No value input");
        }

        if (inputDataDescriptor.getDimensions().getCount() > 0)
        {
            throw std::runtime_error("Arrays not supported");
        }

        inputSampleType = inputDataDescriptor.getSampleType();
        if (inputSampleType != SampleType::Float64 &&
            inputSampleType != SampleType::Float32 &&
            inputSampleType != SampleType::Int8 &&
            inputSampleType != SampleType::Int16 &&
            inputSampleType != SampleType::Int32 &&
            inputSampleType != SampleType::Int64 &&
            inputSampleType != SampleType::UInt8 &&
            inputSampleType != SampleType::UInt16 &&
            inputSampleType != SampleType::UInt32 &&
            inputSampleType != SampleType::UInt64)
        {
            throw std::runtime_error("Invalid sample type");
        }

        // Accept only synchronous (linear implicit) domain signals
        if (inputDomainDataDescriptor.getSampleType() != SampleType::Int64 && inputDomainDataDescriptor.getSampleType() != SampleType::UInt64)
        {
            throw std::runtime_error("Incompatible domain data sample type");
        }

        const auto domainUnit = inputDomainDataDescriptor.getUnit();
        if (domainUnit.getSymbol() != "s" && domainUnit.getSymbol() != "seconds")
        {
            throw std::runtime_error("Domain unit expected in seconds");
        }

        const auto domainRule = inputDomainDataDescriptor.getRule();
        if (inputDomainDataDescriptor.getRule().getType() != DataRuleType::Linear)
        {
            throw std::runtime_error("Domain rule must be linear");
        }

        RangePtr outputRange;
        if (useCustomOutputRange)
        {
            outputRange = Range(outputLowValue, outputHighValue);
        }
        else
        {
            auto outputHigh = scale * static_cast<Float>(inputDataDescriptor.getValueRange().getLowValue()) + offset;
            auto outputLow = scale * static_cast<Float>(inputDataDescriptor.getValueRange().getHighValue()) + offset;
            if (outputLow > outputHigh)
                std::swap(outputLow, outputHigh);

            outputRange = Range(outputLow, outputHigh);
        }

        auto name = outputName.empty() ? inputPort.getSignal().getName().toStdString() + "/Scaled" : outputName;
        auto unit = outputUnit.empty() ? inputDataDescriptor.getUnit() : Unit(outputUnit);

        outputDataDescriptor = DataDescriptorBuilder()
                               .setSampleType(SampleType::Float64)
                               .setValueRange(outputRange)
                               .setUnit(unit)
                               .build();
        outputDomainDataDescriptor = inputDomainDataDescriptor;

        outputSignal.setDescriptor(outputDataDescriptor);
        outputSignal.setName(name);
        outputDomainSignal.setDescriptor(inputDomainDataDescriptor);

        // Allocate 1s buffer
        sampleRate = reader::getSampleRate(inputDomainDataDescriptor);
        inputData.resize(sampleRate);
        inputDomainData.resize(sampleRate);

        setComponentStatus(ComponentStatus::Ok);
    }
    catch (const std::exception& e)
    {
        setComponentStatusWithMessage(ComponentStatus::Error, fmt::format("Failed to set descriptor for output signal: {}", e.what()));
        outputSignal.setDescriptor(nullptr);
        configValid = false;
    }

    configValid = true;
}

void ExampleFBImpl::calculate()
{
    auto lock = this->getAcquisitionLock();

    while (!reader.getEmpty())
    {
        SizeT readAmount = std::min(reader.getAvailableCount(), sampleRate);
        const auto status = reader.readWithDomain(inputData.data(), inputDomainData.data(), &readAmount);

        if (configValid)
        {
            processData(readAmount, status.getOffset());
        }

        if (status.getReadStatus() == ReadStatus::Event)
        {
            const auto eventPacket = status.getEventPacket();
            if (eventPacket.assigned())
                processEventPacket(eventPacket);
            return;
        }
    }
}

void ExampleFBImpl::processData(SizeT readAmount, SizeT packetOffset) const
{
    if (readAmount == 0)
        return;

    const auto outputDomainPacket = DataPacket(outputDomainDataDescriptor, readAmount, packetOffset);
    const auto outputPacket = DataPacketWithDomain(outputDomainPacket, outputDataDescriptor, readAmount);
    auto outputData = static_cast<Float*>(outputPacket.getRawData());

    for (size_t i = 0; i < readAmount; i++)
        *outputData++ = scale * static_cast<Float>(inputData[i]) + offset;

    outputSignal.sendPacket(outputPacket);
    outputDomainSignal.sendPacket(outputDomainPacket);
}

void ExampleFBImpl::processEventPacket(const EventPacketPtr& packet)
{
    if (packet.getEventId() == event_packet_id::DATA_DESCRIPTOR_CHANGED)
    {
        DataDescriptorPtr dataDesc = packet.getParameters().get(event_packet_param::DATA_DESCRIPTOR);
        DataDescriptorPtr domainDesc = packet.getParameters().get(event_packet_param::DOMAIN_DATA_DESCRIPTOR);
        processSignalDescriptorChanged(dataDesc, domainDesc);
    }
}

void ExampleFBImpl::createInputPorts()
{
    inputPort = createAndAddInputPort("Input", PacketReadyNotification::Scheduler);
    reader = StreamReaderFromPort(inputPort, SampleType::Float32, SampleType::UInt64);
    reader.setOnDataAvailable([this] { calculate();});
}

void ExampleFBImpl::createSignals()
{
    outputSignal = createAndAddSignal("Scaled");
    outputDomainSignal = createAndAddSignal("ScaledTime", nullptr, false);
    outputSignal.setDomainSignal(outputDomainSignal);
}

END_NAMESPACE_EXAMPLE_MODULE
