#include <gmock/gmock.h>
#include <testutils/testutils.h>
#include <opendaq/opendaq.h>
#include <opendaq/data_descriptor_factory.h>
#include <thread>

using namespace daq;
using ExampleModuleTest = testing::Test;

TEST_F(ExampleModuleTest, TestAdd)
{
    const auto instance = Instance();
    ASSERT_TRUE(instance.addFunctionBlock("ExampleScalingModule").assigned());
}

TEST_F(ExampleModuleTest, TestPropCount)
{
    const auto instance = Instance();
    auto fb = instance.addFunctionBlock("ExampleScalingModule");
    ASSERT_EQ(fb.getAllProperties().getCount(), 7);
}

TEST_F(ExampleModuleTest, TestDataScaling)
{
    const auto instance = Instance();
    auto fb = instance.addFunctionBlock("ExampleScalingModule");
    fb.setPropertyValue("Scale", 2);

    auto dataDescriptor = DataDescriptorBuilder().setSampleType(SampleType::Float32).setValueRange(Range(-10, 10)).build();
    auto signal = SignalWithDescriptor(instance.getContext(), dataDescriptor, nullptr, "Data");

    const auto domainDescriptor = DataDescriptorBuilder()
                                  .setSampleType(SampleType::Int64)
                                  .setUnit(Unit("s", -1, "seconds", "time"))
                                  .setTickResolution(Ratio(1, 1000))
                                  .setRule(LinearDataRule(1, 0))
                                  .setOrigin("1970-01-01T01:00:00+00:00")
                                  .build();
    auto domainSignal = SignalWithDescriptor(instance.getContext(), domainDescriptor, nullptr, "DomainData");
    signal.setDomainSignal(domainSignal);

    fb.getInputPorts()[0].connect(signal);
    auto streamReader = StreamReader(fb.getSignals()[0], SampleType::Float32, SampleType::Int64);
    
    auto domainPacket = DataPacket(domainDescriptor, 10, 0);
    auto packet = DataPacketWithDomain(domainPacket, dataDescriptor, 10);
    float* data = static_cast<float*>(packet.getRawData());
    for (auto i = 0; i < 10; i++)
        data[i] = static_cast<float>(i);

    signal.sendPacket(packet);
    domainSignal.sendPacket(domainPacket);
    
    SizeT count = 10;
    std::vector<float> readData;
    readData.resize(count);

    auto status = streamReader.read(readData.data(), &count);
    ASSERT_EQ(status.getReadStatus(), ReadStatus::Event);
    ASSERT_EQ(count, 0);

    while (count < 10)
    {
        using namespace std::chrono_literals;
        count = streamReader.getAvailableCount();
        std::this_thread::sleep_for(100ms);
    }

    ASSERT_EQ(count, 10);

    status = streamReader.read(readData.data(), &count);
    ASSERT_EQ(count, 10);
    ASSERT_EQ(status.getReadStatus(), ReadStatus::Ok);

    for (int i = 0; i < 10; i++)
        ASSERT_EQ(static_cast<int>(readData[i]), i * 2);
}
