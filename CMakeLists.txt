cmake_minimum_required(VERSION 3.22)

project(Sonara VERSION 1.0.0)

# Add JUCE
add_subdirectory(JUCE)

juce_add_plugin(Sonara
    COMPANY_NAME "Sonara"
    PLUGIN_NAME "Sonara"
    PLUGIN_CODE Snar
    PLUGIN_VERSION ${PROJECT_VERSION}
    PLUGIN_MANUFACTURER_CODE Sonr
    PLUGIN_DESCRIPTION "Intelligent audio engineering plugin that transforms your sound based on text input"
    FORMATS AU Standalone VST3
    PLUGIN_BANNER_WIDTH 350
    PLUGIN_BANNER_HEIGHT 300
    PRODUCT_ID "Sonara")
    
target_sources(Sonara PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/AudioProcessing/Equalizer.h
    Source/AudioProcessing/Equalizer.cpp
    Source/AudioProcessing/Compressor.h
    Source/AudioProcessing/Compressor.cpp
    Source/AudioProcessing/ReverbProcessor.h
    Source/AudioProcessing/ReverbProcessor.cpp
    Source/KeywordMapper.h
    Source/KeywordMapper.cpp
    Source/ChangesLogger.h
    Source/ChangesLogger.cpp
)

target_compile_definitions(Sonara PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JucePlugin_Build_AU=1
    JucePlugin_Build_VST3=1
    JucePlugin_Build_Standalone=1
)

target_link_libraries(Sonara PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

