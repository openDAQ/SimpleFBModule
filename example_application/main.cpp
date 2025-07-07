/**
 * Server application
 */

#include <iostream>
#include <opendaq/opendaq.h>

using namespace daq;

int main(int /*argc*/, const char* /*argv*/[])
{
    const auto instance = InstanceBuilder().addModulePath(MODULE_PATH).build();
    auto referenceDevice = instance.addDevice("daqref://device0");
    auto renderer = instance.addFunctionBlock("RefFBModuleRenderer");
    auto exampleModule = instance.addFunctionBlock("ExampleScalingModule");
    exampleModule.setPropertyValue("Scale", 3);
    exampleModule.setPropertyValue("Offset", -2);

    exampleModule.getInputPorts()[0].connect(referenceDevice.getSignalsRecursive()[0]);
    renderer.getInputPorts()[0].connect(referenceDevice.getSignalsRecursive()[0]);
    renderer.getInputPorts()[1].connect(exampleModule.getSignals()[0]);

    std::cout << "Press \"enter\" to exit the application..." << std::endl;
    std::cin.get();
    return 0;
}
