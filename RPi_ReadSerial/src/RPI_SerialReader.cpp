#include <RPI_SerialReader.h>


void SerialReader::Write(const std::string& msgOut)
{
    write(m_port, msgOut.c_str(), msgOut.size());
}

tsqueue<std::string>& SerialReader::GetQueue()
{
    return m_msg;
}

void SerialReader::Read()
{
    size_t n = read(m_port, &m_buffer, 256);
    std::string msgIn;
    msgIn.resize(n);
    std::memcpy((void*)msgIn.data(), m_buffer, n);
    m_msg.push_back(msgIn);
}


SerialReader::SerialReader()
{}

SerialReader::~SerialReader()
{
    close(m_port);
}

bool SerialReader::SetDevice(Device device)
{
    // open device
    auto path = GetDevicePath();
    m_port = open(path.c_str(), O_RDWR);
    if (m_port < 0)
    {
        if (m_port == 2)
            std::cerr << "[Error]: No such file or directory: " << path << "\n";
        // sudo adduser $USER dialout
        else if (m_port == 13)
            std::cerr << "[Error]: Can't open device - permission denied!: " << path << "\n";
        return false; 
    }

    // read serial configuration
    struct termios tty;
    if(tcgetattr(m_port, &tty) != 0) 
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    
    // set configuration
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Save tty settings, also checking for error
    if (tcsetattr(m_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    return true;
}

const std::string& SerialReader::GetDevicePath()
{
    if (m_device == Device::Arduino)
        return ARDUINO;
    return ARDUINO;
}


