#include <iostream>
#include <fstream>

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/logging/DefaultLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>
#include <aws/core/utils/HashingUtils.h>

#include <aws/lambda/LambdaClient.h>
#include <aws/lambda/model/InvokeRequest.h>

void InvokeFunction(Aws::String functionName)
{
    Aws::Client::ClientConfiguration clientConfig;
    Aws::Lambda::LambdaClient client(clientConfig);

    Aws::Lambda::Model::InvokeRequest req;
    req.SetFunctionName(functionName);

    req.SetInvocationType(Aws::Lambda::Model::InvocationType::RequestResponse);
    req.SetLogType(Aws::Lambda::Model::LogType::Tail);

    std::shared_ptr<Aws::IOStream> payload = Aws::MakeShared<Aws::StringStream>("FunctionTest");
    Aws::Utils::Json::JsonValue jsonPayload;
    jsonPayload.WithString("key1", "value1");
    jsonPayload.WithString("key2", "value2");
    jsonPayload.WithString("key3", "value3");
    *payload << jsonPayload.View().WriteReadable();

    // AmazonStreamingWebServiceRequest::SetBody( const std::shared_ptr<Aws::IOStream> &body )
    // AmazonStreamingWebServiceRequest::SetContentType( const Aws::String &contentType )

    req.SetBody(payload);
    req.SetContentType("application/javascript");

    // Aws::Utils::Outcome<Aws::Lambda::Model::InvokeResult, Aws::Lambda::LambdaError>
    // outcome = client.Invoke(req);
    auto outcome = client.Invoke(req);
    if (outcome.IsSuccess())
    {
        Aws::Lambda::Model::InvokeResult &result = outcome.GetResult();

        // Lambda function result JSON(key1 value)
        // Aws::IOStream& payload = result.GetPayload();
        Aws::IOStream& outStream = result.GetPayload();
        Aws::String functionResult;
        std::getline(outStream, functionResult);
        std::cout << "Lambda result:\n" << functionResult << std::endl;

        // Decode the result header to see requested log information
        Aws::Utils::Array<unsigned char> byteLogResult =
                Aws::Utils::HashingUtils::Base64Decode(result.GetLogResult());
        Aws::StringStream logResult;
        for (unsigned i = 0; i < byteLogResult.GetLength(); i++)
            logResult << byteLogResult.GetItem(i);
        std::cout << "Log result header:\n" << logResult.str() << std::endl;
    }
    else {
        std::cout << "Error :" << outcome.GetError().AWSError::GetMessage() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage : invoke_function <function>" << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        const Aws::String functionName(argv[1]);
        InvokeFunction(functionName);
    }

    Aws::ShutdownAPI(options);
    return 0;
}
