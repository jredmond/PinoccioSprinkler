/**************************************************************************\
* Pinoccio Library                                                         *
* https://github.com/Pinoccio/library-pinoccio                             *
* Copyright (c) 2014, Pinoccio Inc. All rights reserved.                   *
* ------------------------------------------------------------------------ *
*  This program is free software; you can redistribute it and/or modify it *
*  under the terms of the MIT License as described in license.txt.         *
\**************************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Scout.h>
#include <GS.h>
#include <bitlash.h>
#include <lwm.h>
#include <js0n.h>
#include <Shell.h>

#include "version.h"
#include "OSBee.h"

OSBee osb;

void setup() {
  Scout.setup(SKETCH_NAME, SKETCH_REVISION, SKETCH_BUILD);
  // Add custom setup code here
  addBitlashFunction("sprinkler.begin", (bitlash_function)sprinklerBegin);
  addBitlashFunction("sprinkler.open", (bitlash_function)sprinklerOpen);
  addBitlashFunction("sprinkler.close", (bitlash_function)sprinklerClose);
  addBitlashFunction("custom.wifi", (bitlash_function)customWifi);
  Serial.println("External Aref:");
  Serial.println(String(Scout.getExternalAref()));
}

void loop() {
  Scout.loop();
  // Add custom loop code here
}

numvar sprinklerBegin(void)
{
    Shell.eval("hq.print", "Starting sprinklerTest");
    Serial.println("Starting sprinklerTest");
    osb.begin();
    Shell.eval("hq.print", "Began");
    Serial.println("Began");
}

numvar sprinklerOpen(void)
{
  Serial.println("Starting sprinklerOpen");
  osb.openA();
  Serial.println("Finished sprinkerOpen");
}

numvar sprinklerClose(void)
{
  Serial.println("Starting sprinklerClose");
  osb.closeA();
  Serial.println("Finished sprinklerClose");
}

numvar customWifi(void)
{
    Serial.println("Starting customWifi");
    Shell.eval("wifi.diassociate");
    Shell.eval("wifi.command(\"AT+NDHCP=0\")");
    Shell.eval("wifi.command(\"AT+NSET=192.168.0.220,255.255.255.0,192.168.0.1\")");
    Shell.eval("wifi.command(\"AT+DNSSET=8.8.8.8\")");
    Shell.eval("wifi.reassociate");
    Serial.println("Finished customWifi");
}
