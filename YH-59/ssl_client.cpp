#include "ssl_client.hpp"

bool client::verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx)
{
    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
    std::cout << "Verifying " << subject_name << "\n";
    return preverified;
}

void client::start(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(socket_.lowest_layer(), endpoints,
        boost::bind(&client::handle_connect, this, boost::asio::placeholders::error));
}

void client::handle_connect(const boost::system::error_code& error)
{
    if (!error)
    {
        socket_.async_handshake(boost::asio::ssl::stream_base::client,
        boost::bind(&client::handle_handshake, this, boost::asio::placeholders::error));
    }
    else
    {
       std::cout << "Connect failed: " << error.message() << "\n";
    }
}

void client::handle_handshake(const boost::system::error_code& error)
{
    if (!error)
    {
        size_t request_length = std::strlen(request_);
        boost::asio::async_write(socket_, boost::asio::buffer(request_, request_length),
        boost::bind(&client::handle_send_request, this,
        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else
    {
       std::cout << "Handshake failed: " << error.message() << "\n";
    }
}

void client::handle_send_request(const boost::system::error_code& error, std::size_t length)
{

    if (!error)
    {
        boost::asio::async_read(socket_, boost::asio::buffer(reply_, length),
        boost::bind(&client::handle_receive_response, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        std::cout << "Write failed: " << error.message() << "\n";
    }
}

void client::handle_receive_response(const boost::system::error_code& error, std::size_t len)
{
    if (!error)
    {
        std::cout << "Reply: " << reply_ << std::endl;
    }
    else
    {
        std::cout << "Read failed: " << error.message() << "\n";
    }
}
