Modifications on MFRC522 librairy
In order to assure that the communication of the arduino have successfully passed
There is a a function in the librairy (MFRC522::PCD_DumpVersionToSerial()-Line 1370 in MFRC522.cpp file )

instead of sending an information to Serial we will set on a Led when the com-
munication with the RC522 module have successfully passed
Here is the modification // 

- giving an argument to the function (the led Pin - type byte)
- Set on the led (at the end of function)
- go to the MFRC522.h file and add the argument in function definition

void MFRC522::PCD_DumpVersionToSerial(byte Sig_Pin) {
	// Get the MFRC522 firmware version
	byte v = PCD_ReadRegister(VersionReg);
	Serial.print(F("Firmware Version: 0x"));
	Serial.print(v, HEX);
	// Lookup which version
	switch(v) {
		case 0x88: Serial.println(F(" = (clone)"));  break;
		case 0x90: Serial.println(F(" = v0.0"));     break;
		case 0x91: Serial.println(F(" = v1.0"));     break;
		case 0x92: Serial.println(F(" = v2.0"));     break;
		case 0x12: Serial.println(F(" = counterfeit chip"));     break;
		default:   Serial.println(F(" = (unknown)"));
	}
	// When 0x00 or 0xFF is returned, communication probably failed
	if ((v == 0x00) || (v == 0xFF))
		Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
	else Sig_Pin = 1;  // LINE THAT I HAVE ADDED
} // End PCD_DumpVersionToSerial()
