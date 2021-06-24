#include <iostream>
#include <fstream>
#include <unistd.h>

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/logging/DefaultLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>
#include <aws/core/utils/HashingUtils.h>

#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/GetQueueUrlRequest.h>
#include <aws/sqs/model/GetQueueUrlResult.h>
#include <aws/sqs/model/ReceiveMessageRequest.h>
#include <aws/sqs/model/ReceiveMessageResult.h>

// std::cout << " ReceiptHandle: " << message.GetReceiptHandle() << std::endl;

void recvMessageFunction(Aws::String queueName)
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
        std::cout << rst.GetError().GetMessage() << std::endl;
        return;
    }

    Aws::SQS::Model::ReceiveMessageRequest req;
    req.SetQueueUrl(queue_url);
    req.SetMaxNumberOfMessages(1);

    while ( msg_body != "end" ) {
        // Aws::SQS::Model::ReceiveMessageOutcome rMsgOutcome = sqs.ReceiveMessage(rm_req);
        auto rMsgOutcome = sqs.ReceiveMessage(req);
        if (!rMsgOutcome.IsSuccess()) {
            std::cout << "Error receiving message from queue " << queue_url << ": "
            << rMsgOutcome.GetError().GetMessage() << std::endl;
            break;
        }

        const Aws::Vector<Aws::SQS::Model::Message> &msgVec = rMsgOutcome.GetResult().GetMessages();
        // const auto& messages = rm_out.GetResult().GetMessages();
        if (msgVec.size() == 0) {
            continue;
        }

        std::cout << "Message Rcvd : " << msgVec.size() << std::endl;

        // for ( const auto& message : msgVec) {
        for ( const Aws::SQS::Model::Message &message : msgVec) {
            std::cout << "  Message Id : " << message.GetMessageId() << std::endl;
            std::cout << "Message Body : " << message.GetBody() << std::endl << std::endl;
            msg_body = message.GetBody();
            if ( msg_body == "end" ) {
                break;
            }
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
        recvMessageFunction(queueName);
    }

    Aws::ShutdownAPI(options);
    return 0;
}
