/*
 * Copyright 2022-2024 openDAQ d.o.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <example_module/common.h>
#include <opendaq/function_block_impl.h>
#include <opendaq/opendaq.h>
#include <opendaq/stream_reader_ptr.h>

BEGIN_NAMESPACE_EXAMPLE_MODULE

class ExampleFBImpl final : public FunctionBlock
{
public:
    explicit ExampleFBImpl(const ContextPtr& ctx, const ComponentPtr& parent, const StringPtr& localId);
    ~ExampleFBImpl() override = default;

    static FunctionBlockTypePtr CreateType();

private:
    InputPortPtr inputPort;

    DataDescriptorPtr inputDataDescriptor;
    DataDescriptorPtr inputDomainDataDescriptor;

    DataDescriptorPtr outputDataDescriptor;
    DataDescriptorPtr outputDomainDataDescriptor;

    SampleType inputSampleType;

    SignalConfigPtr outputSignal;
    SignalConfigPtr outputDomainSignal;

    StreamReaderPtr reader;
    SizeT sampleRate;
    std::vector<float> inputData;
    std::vector<uint64_t> inputDomainData;
    
    bool configValid = false;
    Float scale;
    Float offset;
    Float outputHighValue;
    Float outputLowValue;
    Bool useCustomOutputRange;
    std::string outputUnit;
    std::string outputName;

    void createInputPorts();
    void createSignals();

    void calculate();
    void processData(SizeT readAmount, SizeT packetOffset) const;
    void processEventPacket(const EventPacketPtr& packet);

    void processSignalDescriptorChanged(const DataDescriptorPtr& dataDescriptor,
                                        const DataDescriptorPtr& domainDescriptor);
    void configure();

    void initProperties();
    void propertyChanged(bool configure);
    void readProperties();
};

END_NAMESPACE_EXAMPLE_MODULE
