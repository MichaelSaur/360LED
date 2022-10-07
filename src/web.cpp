#include <WiFiNINA.h>
#include <Arduino.h>
#include <globals.h>

WiFiServer server(80);

void sendResponse(WiFiClient client, String path);

void initWebServer(){
    server.begin();
}

void handleWebServer(){
  WiFiClient client = server.available();
   if (client) {
    Serial.println("new client");
    // an HTTP request ends with a blank line
    boolean currentLineIsBlank = true;
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the HTTP request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard HTTP response header
          request = request.substring(request.indexOf("GET ")+4);
          String path = request.substring(0,request.indexOf(" "));
          Serial.println(path);
          sendResponse(client, path);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void sendResponse(WiFiClient client, String path){
    if(path == "/"){
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");  // the connection will be closed after completion of the response
        client.println();
        client.println("<!DOCTYPE HTML>");
        client.println("<html><head><script>function httpGet(){var xmlHttp=new XMLHttpRequest();xmlHttp.open('GET','/preset?' + document.getElementById('preset').value, false );xmlHttp.send( null );return xmlHttp.responseText;}</script></head><body style='background-color:black;color:white'><h1>360 LED Control</h1>");
        client.println("<label for='preset'>Preset</label>");
        client.println("<select id='preset' onchange='httpGet()'>");
        client.println("<option value='-1'>Auto</option>");
        int i;
        for(i=0;i<NUM_ANIMATIONS;i++){
            String selected = "";
            if(i==0 && AUTOMODE){
                selected = "selected";
            }
            if(!AUTOMODE && i+1==animationId){
                String selected = "selected";
            }
            client.println("<option " + selected + "value=" + (String)animations[i].id + ">" + animations[i].name + "</option>");
        }
        client.println("</select>");
        client.println("</body></html>");
    }
    if(path.startsWith("/preset")){
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");  // the connection will be closed after completion of the response
      client.println();
      client.println("<!DOCTYPE HTML>");
      client.println("<html><p>test</p></html>");
      int newID = path.substring(8).toInt();
      if(newID != animationId){
        if(newID==-1){
            AUTOMODE = true;
            Serial.println("changing to auto ");
        }else{
            AUTOMODE = false;
            Serial.println("changing to preset " + path.substring(8));
            animationId = newID;
            broadcastUDP("animation:" + (String)animationId);
        }
      } 
    }
}