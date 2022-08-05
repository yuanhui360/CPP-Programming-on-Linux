#include <crypto++/cryptlib.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <crypto++/base64.h>

#include <iostream>

int main (int argc, char* argv[])
{
    using namespace CryptoPP;

    std::string cipher_str;
    std::string plaintext_str;
    Base64Encoder encoder;
    int msg_size;

    if (argc != 2) {
        fprintf(stdout, "Usage   : %s <message>\n", argv[0]);
        fprintf(stdout, "Example : %s \"this is Hui test security message\"\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     *  1) Message (argv[1]) -> encoder (base64) -> cipher string -> std::cout
     *  2) cipher string  -> decoder (base64) -> plaintext string -> std::cout
     */

    msg_size = strlen(argv[1]);
    byte  *text_buf = (byte*)malloc(msg_size+1);
    memset(text_buf, '\0', msg_size);
    memcpy(text_buf, argv[1], msg_size);

    encoder.Put(text_buf, msg_size);
    encoder.MessageEnd();

    word64 size = encoder.MaxRetrievable();
    if ( size ) {
        cipher_str.resize(size);
        encoder.Get((byte*)&cipher_str[0], cipher_str.size());
    }

    std::cout << "Text Mgs. (" << msg_size << ") : " << argv[1] << std::endl;
    std::cout << "Enc. Msg. (" << size << ") : " << cipher_str;

    /*
     *  Decode cipher data (encodered data)
     *  cipher_string -> decoder (base64) -> plantext_string -> std::cout
     */

    StringSource ss(cipher_str, true, new Base64Decoder( new StringSink(plaintext_str)));
    std::cout << "Dec. Msg. (" << plaintext_str.size() << ") : " << plaintext_str << std::endl;
    free(text_buf);

    return 0;
}
 
