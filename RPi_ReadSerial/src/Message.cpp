#include <Message.h>


MessageHandler::MessageHandler()
{}

MessageHandler::~MessageHandler()
{}

bool MessageHandler::SetDevice(SerialReader::Device device)
{
    return m_reader.SetDevice(device);
}

void MessageHandler::SetMessageHandler(Messages type, std::function<void(float)> f)
{
    m_handler[type] = f;
}

void MessageHandler::Start()
{
    while(!m_stop.load(std::memory_order_relaxed))
    {
        auto& queue = m_reader.GetQueue();
        queue.wait();
        auto msg = queue.pop_front();
        OnMessage(msg);
    }
}

void MessageHandler::Stop()
{
    m_stop = true;
}

void MessageHandler::OnMessage(const std::string& in)
{
    /*
    e.g.
    1 0.58991
    2 0.172345
    4 1
    5 1
    */
    // interprete first byte
    auto t = static_cast<Messages>(in.data()[0]);
    float content;
    if (t < 9)
        content = std::stof(in.substr(1));
    else
        return;

    // call with input
    auto f = m_handler[t];
    f(content);
}