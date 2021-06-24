#include <iostream>
#include <fstream>

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/logging/DefaultLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>
#include <aws/core/utils/HashingUtils.h>

#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/SendMessageRequest.h>
#include <aws/sqs/model/SendMessageResult.h>
#include <aws/sqs/model/GetQueueUrlRequest.h>
#include <aws/sqs/model/GetQueueUrlResult.h>

void sendMessageFunction(Aws::String queueName)
{
    Aws::Client::ClientConfiguration clientConfig;
    Aws::SQS::SQSClient sqs;

    Aws::String queue_url;
    Aws::String msg_body;

    Aws::SQS::Model::GetQueueUrlRequest url_req;
    url_req.SetQueueName(queueName);

    // Aws::Utils::Outcome<Aws::SQS::Model::GetQueueUrlResult, Aws::SQS::SQSError> result
    auto rst = sqs.GetQueueUrl(url_req);
    if (rst.IsSuccess()) {
        queue_url = rst.GetResult().GetQueueUrl();
    }
    else {
        return;
    }

    Aws::SQS::Model::SendMessageRequest sm_req;
    sm_req.SetQueueUrl(queue_url);

    while ( msg_body != Aws::String("end") ) {

        std::getline(std::cin, msg_body);
        sm_req.SetMessageBody(msg_body);

        auto sm_out = sqs.SendMessage(sm_req);
        if (!sm_out.IsSuccess())
        {
            std::cout << "Error : " << sm_out.GetError().GetMessage() << std::endl;
            break;
        }
    }
}

int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        std::cout << "Usage : sqs_sendMsg <Queue Name>" << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        const Aws::String queueName(argv[1]);
        sendMessageFunction(queueName);
    }

    Aws::ShutdownAPI(options);
    return 0;
}
