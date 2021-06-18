AWS DynamoDB is Amazon Cloud non-SQL database, this short video demonstrated how to access AWS DynamoDB 
table and get items with C++ program  on Linux using AWS-SDK for C++ library

******
#include <aws/core/Aws.h>

int main(int argc, char** argv)

{

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        // make your SDK calls here.
    }

    Aws::ShutdownAPI(options);
    return 0;
}
******

Video of AWS DynamicDB :
* AWS DynamoDB access table using AWS-SDK for C++ Library, https://youtu.be/8tcvG8ATzAw
* AWS DynamoDB setup C++ connection on Linux, https://youtu.be/BBOCzgrhCAA
* AWS DynamoDB load data to table from JSON file, https://youtu.be/3D7N1eKgsoo
