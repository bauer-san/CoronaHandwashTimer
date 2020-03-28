// -----------------------------------------
// Target: Particle Photon
//
// Read microphone and turn on LED for 20sec if mic level is GT a threshold
// CDC recommends vigorously washing hands for twenty seconds to prevent spread of diseases and viruses
//
// -----------------------------------------

// In this example, we're going to register a Particle.variable() with the cloud so that we can read amplitude levels from the mic.

int micPIN = A4; 
int ledOnBoard = D7;
int peakToPeak = 0;   // peak-to-peak micLEVEL

const int sampleWindow = 25; // Sample window width in mS (25 mS = 40Hz)
const int twenty_seconds = 20000;

void setup() {

    pinMode(micPIN,INPUT);
    pinMode(ledOnBoard,OUTPUT);
    
    digitalWrite(ledOnBoard, LOW);      // Initial state of on-board LED == off

    Particle.variable("micLEVEL", &peakToPeak, INT);  // declare a Particle.variable() here so that we can read micLEVEL from the cloud.
                                                      // When we ask the cloud for "micLEVEL", this will reference the variable 
                                                      // peakToPeak in this app, which is an integer variable.

}

void loop() {

    unsigned int micLEVEL;    
    unsigned int signalMax = 0;
    unsigned int signalMin = 4096;
    unsigned long start = millis();  // Start of sample window

    // collect data for sampleWindow msec
    while ((millis() - start) < sampleWindow)
    {
        micLEVEL = analogRead(micPIN);
        if (micLEVEL < 4096) { //This is the max of the 12-bit ADC so this loop will include all readings
            if (micLEVEL > signalMax) {
                signalMax = micLEVEL;  // save just the max levels
            } else if (micLEVEL < signalMin) {
                signalMin = micLEVEL;  // save just the min levels
            }
        }
    }
 
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
 
    if (peakToPeak > 2500) {
        digitalWrite(ledOnBoard, HIGH);

        delay(twenty_seconds);
        
        digitalWrite(ledOnBoard, LOW);
        peakToPeak = 0;
    }
}