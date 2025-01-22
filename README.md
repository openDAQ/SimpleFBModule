# Example function block module

Simple example that builds an openDAQ module giving access to an example function block. Said function block scales an input signal with a provided scale, and offsets it by a provided offset.

## Testing the module

To test the module, enable the `OPENDAQ_FB_EXAMPLE_ENABLE_APP` cmake flag. Doing so will add an the openDAQ reference device and function block modules to your project. Those are used to create a simulator device via the "daqref://device0" connection string, as well as a renderer via the "RefFBModuleRenderer" function block ID. The main application connects a reference device signal into both the example scaler and renderer. Additionally, it connects the scaler output into the renderer.

To add additional tests, enable the `EXAMPLE_MODULE_ENABLE_TESTS` cmake flag. Doing so will create a new test target configured for use with the GTest framework.