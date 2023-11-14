  // Set sensor ports 
const int OPTO = 2, AMP = A2, VOLT = A1;

// Initialize variables 
volatile int value = 0;
volatile bool toggleState = false;
volatile float rpm, volt, current;


void setup()
{
  Serial.begin(9600); 
  pinMode(OPTO, INPUT); // Set Optocoupler as INPUT
  pinMode(AMP, INPUT); // Set Current as INPUT
  pinMode(VOLT, INPUT); // Set voltage as INPUT
  pinMode(13, OUTPUT);

//set timer4 interrupt at 1Hz
  TCCR4A = 0x00; 
  TCCR4B = 0x00; 
  OCR4A = 0x3D08; // 16MHz/1024/1Hz - 1 = 15624 | Time for period: 1 seconds
  TCCR4B |= (1 << WGM12)|(1<<CS10)|(1<<CS12); // CTC Mode & Prescaler 1024; 

  TIMSK4 |= (1 << OCIE4A);
  value = 0;
  attachInterrupt(digitalPinToInterrupt(OPTO), count, RISING); // Count number of falling in a period
}


void count()
{
  value++; // Increase number of fallings
}


ISR(TIMER4_COMPA_vect)
{
  rpm = value;
  toggleState = !toggleState;
  digitalWrite(13, toggleState);

  volt = (analogRead(VOLT) * 5.0) / 1024.0; // Es necesario tomar un promedio del voltaje
  current = (analogRead(AMP) * 5.0) / 1024.0; // Igualmente es necesario tomar un promedio de corriente durante el tiempo designado.

  // Generar cadena de caracteres con especios fijos y enviar mediante Serial.write() para ahorrar espacio en memoria.
  Serial.print(volt);
  Serial.print(";");
  Serial.print(current);
  Serial.print(";");
  Serial.println(rpm);

  value = 0;
}

void loop()
{

}



