// Esp32 working example https://github.com/mandulaj/PZEM-004T-v30/issues/64




#include <PZEM004Tv30.h>

//PZEM004Tv30 pzem1(&Serial1,15,4,0xE1);
//PZEM004Tv30 pzem2(&Serial1,15,4,0xE2);
PZEM004Tv30 pzem3(&Serial2,27,14,0xE3);
PZEM004Tv30 pzem4(&Serial2,27,14,0xE4);
//PZEM004Tv30 pzem5(&Serial2,27,14,0xE5);
//PZEM004Tv30 pzem6(&Serial2,27,14,0xE6);
//PZEM004Tv30 pzem7(&Serial2,27,14,0xE7);

//multiply
float VMF_Factor = 2.0;
float WMF_Factor = 2.0;
//substract
float VSF_Factor = 0.20;
float WSF_Factor = 0.30;


void setup() {
  Serial.begin(115200);
}

void loop() {
// Serial.println("PZEM1=====================");
//   float voltage = pzem1.voltage();
//   
//   if(!isnan(voltage)){
//       Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
//       byte ok = 1;
//       Serial.print("OK: "); Serial.print(ok);
//   } else {
//       Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
//       Serial.println("Error reading voltage");
//       byte ok = 0;
//       Serial.print("OK: "); Serial.println(ok);
//   }
//
//   float current = pzem1.current();
//   if(!isnan(current)){
//       Serial.print("Current: "); Serial.print(current); Serial.println("A");
//   } else {
//       Serial.println("Error reading current");
//   }
//
//   float power = pzem1.power();
//   if(!isnan(power)){
//       Serial.print("Power: "); Serial.print(power); Serial.println("W");
//   } else {
//       Serial.println("Error reading power");
//   }
//
//   float energy = pzem1.energy();
//   if(!isnan(energy)){
//       Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
//   } else {
//       Serial.println("Error reading energy");
//   }
//
//   float frequency = pzem1.frequency();
//   if(!isnan(frequency)){
//       Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
//   } else {
//       Serial.println("Error reading frequency");
//   }
//
//   float pf = pzem1.pf();
//   if(!isnan(pf)){
//       Serial.print("PF: "); Serial.println(pf);
//   } else {
//       Serial.println("Error reading power factor");
//   }
//
//   Serial.println();
//   Serial.println("PZEM2=====================");
//
//   voltage = pzem2.voltage();
//   if(!isnan(voltage)){
//       Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
//   } else {
//    voltage= 0;
//    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
////       Serial.println("Error reading voltage");
//   }
//
//   current = pzem2.current();
//   if(!isnan(current)){
//       Serial.print("Current: "); Serial.print(current); Serial.println("A");
//   } else {
//       Serial.println("Error reading current");
//   }
//
//   power = pzem2.power();
//   if(!isnan(power)){
//       Serial.print("Power: "); Serial.print(power); Serial.println("W");
//   } else {
//       Serial.println("Error reading power");
//   }
//
//   energy = pzem2.energy();
//   if(!isnan(energy)){
//       Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
//   } else {
//       Serial.println("Error reading energy");
//   }
//
//   frequency = pzem2.frequency();
//   if(!isnan(frequency)){
//       Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
//   } else {
//       Serial.println("Error reading frequency");
//   }
//
//   pf = pzem2.pf();
//   if(!isnan(pf)){
//       Serial.print("PF: "); Serial.println(pf);
//   } else {
//       Serial.println("Error reading power factor");
//   }

   Serial.println();
   Serial.println("PZEM3=====================");

   float voltage = pzem3.voltage();
   if(!isnan(voltage)){
       Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
   } else {
       Serial.println("Error reading voltage");
   }

  float current = pzem3.current();
   if(!isnan(current)){
       Serial.print("Current: "); Serial.print(current); Serial.println("A");
   } else {
       Serial.println("Error reading current");
   }

   float power = pzem3.power();
   if(!isnan(power)){
       Serial.print("Power: "); Serial.print(power); Serial.println("W");
   } else {
       Serial.println("Error reading power");
   }

  float energy = pzem3.energy();
   if(!isnan(energy)){
       Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
   } else {
       Serial.println("Error reading energy");
   }

   float frequency = pzem3.frequency();
   if(!isnan(frequency)){
       Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
   } else {
       Serial.println("Error reading frequency");
   }

   float pf = pzem3.pf();
   if(!isnan(pf)){
       Serial.print("PF: "); Serial.println(pf);
   } else {
       Serial.println("Error reading power factor");
   }

   Serial.println();
   Serial.println("PZEM4=====================");

   voltage  = VMF_Factor * pzem4.voltage();

  voltage=  voltage - VSF_Factor;

   if(!isnan(voltage)){
       Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
   } else {
       Serial.println("Error reading voltage");
   }

   current = pzem4.current();
   if(!isnan(current)){
       Serial.print("Current: "); Serial.print(current); Serial.println("A");
   } else {
       Serial.println("Error reading current");
   }

   power = WMF_Factor * pzem4.power();

   power = power + VSF_Factor;
   
   if(!isnan(power)){
       Serial.print("Power: "); Serial.print(power); Serial.println("W");
   } else {
       Serial.println("Error reading power");
   }

   energy = pzem4.energy();
   if(!isnan(energy)){
       Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
   } else {
       Serial.println("Error reading energy");
   }

   frequency = pzem4.frequency();
   if(!isnan(frequency)){
       Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
   } else {
       Serial.println("Error reading frequency");
   }

   pf = pzem4.pf();
   if(!isnan(pf)){
       Serial.print("PF: "); Serial.println(pf);
   } else {
       Serial.println("Error reading power factor");
   }

   Serial.println();
//   Serial.println("PZEM5=====================");
//
//   voltage = pzem5.voltage();
//   if(!isnan(voltage)){
//       Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
//   } else {
//       Serial.println("Error reading voltage");
//   }
//
//   current = pzem5.current();
//   if(!isnan(current)){
//       Serial.print("Current: "); Serial.print(current); Serial.println("A");
//   } else {
//       Serial.println("Error reading current");
//   }
//
//   power = pzem5.power();
//   if(!isnan(power)){
//       Serial.print("Power: "); Serial.print(power); Serial.println("W");
//   } else {
//       Serial.println("Error reading power");
//   }
//
//   energy = pzem5.energy();
//   if(!isnan(energy)){
//       Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
//   } else {
//       Serial.println("Error reading energy");
//   }
//
//   frequency = pzem5.frequency();
//   if(!isnan(frequency)){
//       Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
//   } else {
//       Serial.println("Error reading frequency");
//   }
//
//   pf = pzem5.pf();
//   if(!isnan(pf)){
//       Serial.print("PF: "); Serial.println(pf);
//   } else {
//       Serial.println("Error reading power factor");
//   }
//
//   Serial.println();
//   Serial.println("PZEM6=====================");
//
//   voltage = pzem6.voltage();
//   if(!isnan(voltage)){
//       Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
//   } else {
//       Serial.println("Error reading voltage");
//   }
//
//   current = pzem6.current();
//   if(!isnan(current)){
//       Serial.print("Current: "); Serial.print(current); Serial.println("A");
//   } else {
//       Serial.println("Error reading current");
//   }
//
//   power = pzem6.power();
//   if(!isnan(power)){
//       Serial.print("Power: "); Serial.print(power); Serial.println("W");
//   } else {
//       Serial.println("Error reading power");
//   }
//
//   energy = pzem6.energy();
//   if(!isnan(energy)){
//       Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
//   } else {
//       Serial.println("Error reading energy");
//   }
//
//   frequency = pzem6.frequency();
//   if(!isnan(frequency)){
//       Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
//   } else {
//       Serial.println("Error reading frequency");
//   }
//
//   pf = pzem6.pf();
//   if(!isnan(pf)){
//       Serial.print("PF: "); Serial.println(pf);
//   } else {
//       Serial.println("Error reading power factor");
//   }
//
//   Serial.println();
//   Serial.println("PZEM7=====================");
//
//   voltage = pzem7.voltage();
//   if(!isnan(voltage)){
//       Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
//   } else {
//       Serial.println("Error reading voltage");
//   }
//
//   current = pzem7.current();
//   if(!isnan(current)){
//       Serial.print("Current: "); Serial.print(current); Serial.println("A");
//   } else {
//       Serial.println("Error reading current");
//   }
//
//   power = pzem7.power();
//   if(!isnan(power)){
//       Serial.print("Power: "); Serial.print(power); Serial.println("W");
//   } else {
//       Serial.println("Error reading power");
//   }
//
//   energy = pzem7.energy();
//   if(!isnan(energy)){
//       Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
//   } else {
//       Serial.println("Error reading energy");
//   }
//
//   frequency = pzem7.frequency();
//   if(!isnan(frequency)){
//       Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
//   } else {
//       Serial.println("Error reading frequency");
//   }
//
//   pf = pzem7.pf();
//   if(!isnan(pf)){
//       Serial.print("PF: "); Serial.println(pf);
//   } else {
//       Serial.println("Error reading power factor");
//   }

   Serial.println();
   
   delay(1000);

}
