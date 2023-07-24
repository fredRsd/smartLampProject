#include <ESP8266WiFi.h>  //Wifi Library

const char* ssid = "nmcuW";           //wifi SSID
const char* password = "ourGroup@3215";  //Wi-fi password

int bluePin = 16; //GPIO16 pin connected to blue
int redPin = 5; //GPIO5 pin connected to blue
int greenPin = 4; //GPIO4 pin connected to blue
WiFiServer server(80);  //sets wifi server

/*initial pin setup function*/
void initLights(){
  pinMode(bluePin, OUTPUT); //output for pin connected to blue light 
  pinMode(greenPin, OUTPUT);  //output for pin connected to green light 
  pinMode(redPin, OUTPUT);  //output for pin connected to blue light 
  digitalWrite(bluePin, LOW); //blue pin initial value set to LOW(off)
  digitalWrite(redPin, LOW);  //red Pin pin initial value set to LOW(off)
  digitalWrite(greenPin, LOW);  //green pin initial value set to LOW(off)
}

/*function to initiate wifi connection*/
void wifiConnect(){
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP()); //address in the local server
  Serial.println("/");
}

/*function to connect pins for given rgb code */
void turnOnColor(int r, int g, int b){
  digitalWrite(redPin, LOW);  //red pin originnaly set to LOW
  digitalWrite(greenPin, LOW);  //green pin set to LOW
  digitalWrite(bluePin, LOW); //blue pin set to LOW
  if (r == 1) digitalWrite(redPin, HIGH); //Turns on GPIO connected to red if requested
  if (g == 1) digitalWrite(greenPin, HIGH); //Turns on GPIO connected to green if requested
  if (b == 1) digitalWrite(bluePin, HIGH);  //Turns on GPIO connected to red if requested
}

void setup() {
  Serial.begin(115200); //serial begins
  delay(10);  //0.01 seconds delay
  initLights(); //initialize the light pins
  wifiConnect();  //initialize wifi
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
    return;

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available())
    delay(1);

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the requested color
  if (request.indexOf("/LED=OFF") != -1)
    turnOnColor(0, 0, 0);
  if (request.indexOf("/LED=ON") != -1)
    turnOnColor(1, 1, 1);
  if (request.indexOf("/LED=RED") != -1)
    turnOnColor(1, 0, 0);
  if (request.indexOf("/LED=GREEN") != -1)
    turnOnColor(0, 1, 0);
  if (request.indexOf("/LED=BLUE") != -1)
    turnOnColor(0, 0, 1);
  if (request.indexOf("/LED=CYAN") != -1)
    turnOnColor(0, 1, 1);
  if (request.indexOf("/LED=MAGENTA") != -1)
    turnOnColor(1, 0, 1);
  if (request.indexOf("/LED=YELLOW") != -1)
    turnOnColor(1, 1, 0);
  if (request.indexOf("/LED=WHITE") != -1)
    turnOnColor(1, 1, 1);
  if (request.indexOf("/LED=AMAZING") != -1) {
    int count = 0;
    while(count<2){
      turnOnColor(1,0,0);
      delay(250);
      turnOnColor(1,1,0);
      delay(250);
      turnOnColor(0,1,0);
      delay(250);
      turnOnColor(0,1,1);
      delay(250);
      turnOnColor(0,0,1);
      delay(250);
      turnOnColor(1,0,1);
      delay(250);
      count += 1;
    }
  }

  // Return the response + HTML Handle
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");  //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("Led pin is now: ");
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button> ON </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button> OFF </button></a><br />");
  client.println("<a href=\"/LED=RED\"\"><button> Red </button></a><br />");
  client.println("<a href=\"/LED=GREEN\"\"><button> Green </button></a><br />");
  client.println("<a href=\"/LED=BLUE\"\"><button> Blue </button></a><br />");
  client.println("<a href=\"/LED=CYAN\"\"><button> Cyan </button></a><br />");
  client.println("<a href=\"/LED=MAGENTA\"\"><button> Magenta </button></a><br />");
  client.println("<a href=\"/LED=YELLOW\"\"><button> Yellow </button></a><br />");
  client.println("<a href=\"/LED=WHITE\"\"><button> White </button></a><br />");
  client.println("<a href=\"/LED=AMAZING\"\"><button> Amazing </button></a><br />");
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
