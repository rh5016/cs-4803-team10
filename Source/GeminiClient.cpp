#include "GeminiClient.h"
#include <juce_core/juce_core.h>

GeminiClient::GeminiClient() : Thread("GeminiAPIThread")
{
    startThread();
}

GeminiClient::~GeminiClient()
{
    stopThread(5000); // Wait up to 5 seconds for thread to finish
}

void GeminiClient::setApiKey(const juce::String& key)
{
    apiKey = key.trim();
}

void GeminiClient::processTextAsync(const juce::String& userInput, ResponseCallback callback)
{
    if (!isApiKeySet())
    {
        if (callback)
            callback(false, userInput, "API key not set. Please configure your Gemini API key.");
        return;
    }
    
    {
        const juce::ScopedLock lock(requestLock);
        currentRequest.input = userInput;
        currentRequest.callback = callback;
        hasRequest = true;
    }
    
    requestReady.signal();
}

bool GeminiClient::processTextSync(const juce::String& userInput, juce::String& processedText)
{
    if (!isApiKeySet())
    {
        lastError = "API key not set";
        processedText = userInput;
        return false;
    }
    
    return makeGeminiRequest(userInput, processedText);
}

void GeminiClient::run()
{
    while (!threadShouldExit())
    {
        // Wait for a request
        requestReady.wait(100);
        
        Request request;
        bool shouldProcess = false;
        
        {
            const juce::ScopedLock lock(requestLock);
            if (hasRequest)
            {
                request = currentRequest;
                hasRequest = false;
                shouldProcess = true;
            }
        }
        
        if (shouldProcess && request.callback)
        {
            juce::String processedText;
            bool success = makeGeminiRequest(request.input, processedText);
            
            if (success)
            {
                request.callback(true, processedText, "");
            }
            else
            {
                // On error, return original input as fallback
                request.callback(false, request.input, lastError);
            }
        }
    }
}

bool GeminiClient::makeGeminiRequest(const juce::String& input, juce::String& output)
{
    lastError.clear();
    
    // Build the API URL (without POST data in URL)
    // Using Gemini 1.5 Flash (free tier) for faster responses
    juce::String urlString = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + apiKey;
    juce::URL url(urlString);
    
    // Build the request payload
    juce::String prompt = buildPrompt(input);
    
    // Escape JSON string properly
    juce::String escapedPrompt = prompt.replace("\\", "\\\\")
                                       .replace("\"", "\\\"")
                                       .replace("\n", "\\n")
                                       .replace("\r", "\\r")
                                       .replace("\t", "\\t");
    
    juce::String jsonPayload = "{\n"
        "  \"contents\": [{\n"
        "    \"parts\": [{\n"
        "      \"text\": \"" + escapedPrompt + "\"\n"
        "    }]\n"
        "  }],\n"
        "  \"generationConfig\": {\n"
        "    \"temperature\": 0.3,\n"
        "    \"maxOutputTokens\": 500\n"
        "  }\n"
        "}";
    
    // Create POST data as MemoryBlock
    juce::MemoryBlock postDataBlock;
    postDataBlock.append(jsonPayload.toRawUTF8(), jsonPayload.getNumBytesAsUTF8());
    
    // Set POST data on URL
    url = url.withPOSTData(postDataBlock);
    
    // Create input stream which will automatically use POST when POST data is set
    auto inputStream = url.createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inPostData)
        .withExtraHeaders("Content-Type: application/json\r\n")
        .withConnectionTimeoutMs(10000)
        .withNumRedirectsToFollow(3));
    
    if (inputStream == nullptr)
    {
        lastError = "Failed to connect to Gemini API. Check your internet connection and API key.";
        return false;
    }
    
    // Read response
    juce::String responseText = inputStream->readEntireStreamAsString();
    
    if (responseText.isEmpty())
    {
        lastError = "Empty response from Gemini API";
        return false;
    }
    
    // Parse the JSON response
    return parseGeminiResponse(responseText, output);
}

juce::String GeminiClient::buildPrompt(const juce::String& userInput)
{
    return juce::String("You are an audio engineering assistant. Your task is to convert the user's natural language request into standardized audio engineering keywords that describe what they want.\n\n")
        + "Available keyword categories:\n"
        + "- Brightness: bright, airy, sparkle, clarity, crisp, presence, shine, clear, detailed\n"
        + "- Warmth: warm, smooth, body, full, thick, round, mellow, soft, sweet\n"
        + "- Reverb: reverb, room, space, spacious, hall, ambience, ambient, echo, wet, atmosphere\n"
        + "- Compression: punch, punchy, tight, glue, cohesion, consistent, control, level, even\n"
        + "- Bass: bass, low end, lows, deep, boom, thump, kick, weight, heavy\n"
        + "- Presence: presence, forward, upfront, cut, vocal, mids, midrange, snap\n\n"
        + "User's request: \"" + userInput + "\"\n\n"
        + "Instructions:\n"
        + "1. Extract the audio engineering intent from the user's request\n"
        + "2. Convert it to keywords from the categories above\n"
        + "3. Add intensity modifiers if mentioned (slight, more, very, less)\n"
        + "4. Return ONLY the processed keywords/phrases, nothing else\n"
        + "5. Keep it concise - maximum 50 words\n"
        + "6. If the request doesn't relate to audio, return \"[INVALID]\"\n\n"
        + "Processed keywords:";
}

bool GeminiClient::parseGeminiResponse(const juce::String& jsonResponse, juce::String& processedText)
{
    try
    {
        // Parse JSON using JUCE's JSON parser
        auto json = juce::JSON::parse(jsonResponse);
        
        if (!json.isObject())
        {
            lastError = "Invalid JSON response format";
            return false;
        }
        
        auto candidates = json.getProperty("candidates", juce::var());
        if (!candidates.isArray() || candidates.getArray()->size() == 0)
        {
            lastError = "No candidates in response";
            return false;
        }
        
        auto firstCandidate = candidates.getArray()->getFirst();
        if (!firstCandidate.isObject())
        {
            lastError = "Invalid candidate format";
            return false;
        }
        
        auto content = firstCandidate.getProperty("content", juce::var());
        if (!content.isObject())
        {
            lastError = "Invalid content format";
            return false;
        }
        
        auto parts = content.getProperty("parts", juce::var());
        if (!parts.isArray() || parts.getArray()->size() == 0)
        {
            lastError = "No parts in content";
            return false;
        }
        
        auto firstPart = parts.getArray()->getFirst();
        if (!firstPart.isObject())
        {
            lastError = "Invalid part format";
            return false;
        }
        
        auto text = firstPart.getProperty("text", juce::var());
        if (!text.isString())
        {
            lastError = "No text in response";
            return false;
        }
        
        processedText = text.toString().trim();
        
        // Check if Gemini returned [INVALID]
        if (processedText.contains("[INVALID]"))
        {
            lastError = "Request doesn't relate to audio processing";
            return false;
        }
        
        return true;
    }
    catch (...)
    {
        lastError = "Failed to parse JSON response";
        return false;
    }
}

