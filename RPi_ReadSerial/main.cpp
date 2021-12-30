#include <RPI_SerialReader.h>
#include <Message.h>

void process_brightness(float brightness)
{
    std::cout << "New Brightness: " << brightness << "\n";
}

int main()
{
    MessageHandler handler;
    handler.SetDevice(SerialReader::Device::Arduino);
    handler.SetMessageHandler(Messages::Brightness, process_brightness);

    std::thread arduino_reader([&handler](){
        handler.Start();
    });

    std::this_thread::sleep_for(std::chrono::seconds(10));
    handler.Stop();

    if(arduino_reader.joinable())
        arduino_reader.join();

    return 0;
}