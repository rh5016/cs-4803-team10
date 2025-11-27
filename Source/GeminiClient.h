#pragma once

#include <juce_core/juce_core.h>
#include <functional>

/**
 * Client for Google Gemini API integration.
 * Handles HTTP requests to the Gemini API and processes responses.
 */
class GeminiClient : public juce::Thread
{
public:
    /**
     * Callback function type for async API responses.
     * Parameters: (success, processedText, errorMessage)
     */
    using ResponseCallback = std::function<void(bool, const juce::String&, const juce::String&)>;
    
    GeminiClient();
    ~GeminiClient() override;
    
    /**
     * Set the Gemini API key.
     * Get your free API key from: https://aistudio.google.com/api-keys
     */
    void setApiKey(const juce::String& apiKey);
    
    /**
     * Check if API key is configured.
     */
    bool isApiKeySet() const { return apiKey.isNotEmpty(); }
    
    /**
     * Process text using Gemini API asynchronously.
     * The Gemini API will normalize and enhance the user's natural language input
     * into audio engineering keywords that the KeywordMapper can understand.
     * 
     * @param userInput The original user text input
     * @param callback Function to call when processing completes
     */
    void processTextAsync(const juce::String& userInput, ResponseCallback callback);
    
    /**
     * Synchronous version - blocks until response is received.
     * Use with caution as it will block the calling thread.
     * 
     * @param userInput The original user text input
     * @param processedText Output parameter for the processed text
     * @return true if successful, false otherwise
     */
    bool processTextSync(const juce::String& userInput, juce::String& processedText);
    
    /**
     * Get the last error message, if any.
     */
    juce::String getLastError() const { return lastError; }
    
private:
    void run() override;
    
    // Internal request structure
    struct Request
    {
        juce::String input;
        ResponseCallback callback;
    };
    
    juce::String apiKey;
    juce::String lastError;
    juce::WaitableEvent requestReady;
    juce::CriticalSection requestLock;
    Request currentRequest;
    bool hasRequest = false;
    
    /**
     * Make HTTP request to Gemini API.
     */
    bool makeGeminiRequest(const juce::String& input, juce::String& output);
    
    /**
     * Build the prompt for Gemini to process audio engineering requests.
     */
    juce::String buildPrompt(const juce::String& userInput);
    
    /**
     * Parse JSON response from Gemini API.
     */
    bool parseGeminiResponse(const juce::String& jsonResponse, juce::String& processedText);
};

