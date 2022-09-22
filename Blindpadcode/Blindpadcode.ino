//  Create a define to allow No Op 1 clock cycle delays to be used
#define NOP __asm__ __volatile__ ("nop\n\t")

//  Pin numbers for the data, clock and latch outputs respectively
#define dataPin 4
#define clockPin 2
#define latchPin 3

//  Define the chars that will be used for the serial communication
#define startChar '/'
#define identifier 'v'
#define questionChar '?'
#define endChar '!'

char hexData[] = {"0123456789ABCDEF"};

char state = 0; // store which part of the square wave was sent

//  Create the variables used by the program to know size and data for the output
const int dataLength = 8; //  This is the number of shift registers that program will run
char output[dataLength] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x00, 0x55};
char blankOutput[dataLength];

int freq = 100;  //  running frequency of the taxels (Hz)
int period;

int timer1_counter; //  define a counter for the timer interrupt

// create variables that will be used while reading serial data input
char rxFlag = 0;
char messageStartFlag = 0;
char message[dataLength * 2];
int messagePos = 0;

void setup() 
{
  // Initialise the serial port at a baud rate of 115200
  Serial.begin(115200);
  
  // Set up the output pins, then set each of their outputs to 0
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  digitalWrite(dataPin, 0);
  digitalWrite(clockPin, 0);
  digitalWrite(latchPin, 0);

  //  Set up the blank output with all 0s
  for (int i = 0; i < dataLength; i++)
  {
    blankOutput[i] = 0x00;
  }

  //  Set up the timer interrupt to run at 2 times the set output frequency
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  timer1_counter = 65536 - (16000000 / 256 / freq );
  TCNT1 = timer1_counter;
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << TOIE1);
  interrupts();
}

//  This is the Interrupt Service Routine for the timer interrupt. This exact timing is needed 
//  to create the square wave output at the desired frequency
ISR(TIMER1_OVF_vect)
{
  //  Reset the timer counter
  TCNT1 = timer1_counter;

  // latch the output on the interrupt to create a square wave of known frequency
  latchOutput();
  
  NOP;  
  
  //  send the next data to the shift registers
  if (state) sendData(output);
  else  sendData(blankOutput);
  state = !state;
  
  // CODE IN THE TIMER INTERRUPT HANDLER
  // if a command was received while waiting, handle it now
  if (rxFlag == 1)  //  handle the command that changed the data on the taxel array
  {
    rxFlag = 0; // clear the receive flag

    Serial.print("/v"); //  send the initial data identifier back to the computer

    unsigned char temp;
    for (int pos = 0; pos < (dataLength); pos++)  //  loop over each byte in the data
    {
      temp = ascii2hex(message[pos * 2]);
      temp = (temp << 4) | ascii2hex(message[(pos * 2) + 1]);
      output[pos] = temp; //  set the internally stored output to the input from the message
      
      Serial.print(hexData[(output[pos] >> 4) & 0xf]);  //  print the data back to the computer
      Serial.print(hexData[output[pos] & 0xf]);
    }
    Serial.println(""); //  end the transmission wiht a new line
  }
  else if (rxFlag == 2) //  handle the /?\n command
  {
    //  Send the response 
    Serial.print("/vd");
    Serial.println(hexData[dataLength & 0x0f]);
    rxFlag = 0;
  }
}

//  Main loop of the program. This continually checks for serial data on the input and reads 
//  from it if it exists.
void loop() 
{
  //  if data is available, and its already received a full command, start recording
  if ((Serial.available() > 0) && rxFlag == 0)  
  {
    char messageChar = Serial.read();

    if ((messageChar == startChar) && (messageStartFlag == 0))  //  checks to see if the data is a '/'
    {
      messageStartFlag = 1;
    }
    else if ((messageChar == identifier) && (messageStartFlag == 1))  //  checks to see if the data is 'v'
    {
      messageStartFlag = 2;
    }
    else if ((messageChar == questionChar) && (messageStartFlag == 1))  //  checks to see if the data is '?'
    {
      messageStartFlag = 3;
    }
    else if ((messageStartFlag == 2) && (messageChar == endChar)) //  check to see if the data is '\n' after receiving data
    {
      rxFlag = 1;
      messageStartFlag = 0;
      messagePos = 0;
    }
    else if ((messageStartFlag == 2) && (messageChar != endChar)) //  if the data is not '\n' and '/v' has been received, record the data
    {
      message[messagePos] = messageChar;
      messagePos++;
    }
    else if ((messageStartFlag == 3) && (messageChar == endChar)) //  check to see if the data is '\n' after receiving '/?'
    {
      rxFlag = 2;
      messageStartFlag = 0;
      messagePos = 0;
    }
  }
}

//  Function that sends a very short pulse to the latch on the shift registers
void latchOutput()
{
  digitalWrite(latchPin,1);
  NOP;
  digitalWrite(latchPin, 0);
}

//  Function that can take a data array, and byte by byte send it to the shift registers
void sendData(char *data)
{
  for (int j = 0; j < dataLength; j++)  //  send each byte in the output to the sendChar() function
  {
    sendChar(*data);
    data++;
  }
}

//  Function that sends the data from a single byte to the shift registers
void sendChar(char packet)
{
  for (int i = 0; i < 8; i++)
  {
    // for each bit of data, show it on the output pin, then clock the output. 
    digitalWrite(dataPin, packet & 0x01);
    packet = packet >> 1;
    digitalWrite(clockPin, HIGH);
    NOP;
    digitalWrite(clockPin, LOW);
    NOP;
  }
}


unsigned char ascii2hex(unsigned char c)
{
    if (c > 0x40 && c < 0x47)       c = c - 0x37; //  upper case letters
    else if (c > 0x60 && c < 0x67)  c = c - 0x57; //  lower case letters
    c = c & 0x0f; 
    return c;
}
