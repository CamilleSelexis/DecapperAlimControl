void homePage(EthernetClient *client_pntr){
  Serial.println("home");
  long current_time = millis();
  int seconds = (int) (current_time / 1000) % 60 ;
  int minutes = (int) ((current_time / (1000*60)) % 60);
  int hours   = (int) ((current_time / (1000*60*60)) % 24);
  char c[30];
  int l = sprintf(c, "%02d:%02d:%02d",hours,minutes,seconds);
  client_pntr->println(F("HTTP/1.1 200 OK"));
  client_pntr->println(F("Content-Type: text/html"));
  client_pntr->println(F("Connection: close"));  // the connection will be closed after completion of the response
  client_pntr->println();
  client_pntr->println(F("<!DOCTYPE HTML>"));
  client_pntr->println(F("<html>"));
  client_pntr->println(F("<body>"));
  client_pntr->print("<h1 style=\"text-align:center\">Decapper Power Controller  </h1>");
  client_pntr->print("<p> Power : ");
  if(powerOn) client_pntr->println("<strong style= \"background-color:#00ff00\"> ON </strong>");
  else client_pntr->println("<strong style= \"background-color:#ff0000\"> OFF </strong>");
  client_pntr->println(F("<p><a href=\"http://192.168.1.107/powerOn\">Enable the decappers</a></p>"));
  client_pntr->println(F("<p><a href=\"http://192.168.1.107/powerOff\">Disable the decappers</a></p>"));
  client_pntr->print("<p> Connection closed by the server at internal time : ");client_pntr->print(millis());
  //Close the connection
  client_pntr->print("</p> </body>");
  client_pntr->print("</html>");
  delay(10);
  //client_pntr->flush();
  delay(500);
  //while (client_pntr->read() != -1);
  ////Serial.println("Client stop called");
  client_pntr->stop();
}
