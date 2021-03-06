#include <LiquidCrystal_I2C.h>
#define PIN_LED A0                // High side of the resistor
#define PIN_MEDICION A6           // Low side of resistor and anode of LED
#define DIFERENCIA_MINIMA 19.00           
#define CANTIDAD_SENSADA 10
#define CANTIDAD_PALABRAS 40

// Variables globales
LiquidCrystal_I2C lcd(0x27,8,2);
long int dataSensada[CANTIDAD_SENSADA];
long int dataSensada2[CANTIDAD_SENSADA];
int j = 0;
int k = 0;
String palabras[CANTIDAD_PALABRAS] = {
"fatigada","la noche","estoy mirando","en alto","tal dulzura","no preguntes","noche","ojos...","luna blanca","puedo escapar",
"cielo profundo","afuera llueve","por la vida","corazon...","tus brazos","sin hacer ruido","salvame","era silencio","maquina","de un suspiro",
"la mirada","no entender","mueres de sed","tus pupilas","perfume tenue","Oh viajero","y dile","huye hacia","de esta fiebre","corre, camina",
"todo eso que","pienso que","Hasta cuando?","el sol","para tu suerte","recuerdos de","el alma","como de mujer","a mirarlas","contemplo el mar"};

void setup() {
  // Inicializa display
  lcd.init();
  lcd.clear();

  // Inicializa led
  Serial.begin(230400);                     
  pinMode(PIN_MEDICION, INPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);   
}

void loop() {

  long int sum = 0;

  // Suma 256 lecturas para reducir el ruido
  for (int i = 0; i < 256; i++) {
    sum = sum + analogRead(PIN_MEDICION);
  }

  // Guarda primero 10 mediciones, luego otras 10
  if(j < CANTIDAD_SENSADA) {
    dataSensada[j] = sum; 
    j++;
  } else {
    dataSensada2[k] = sum; 
    k++;
  }

  // Si ya están llenos los 2 arrays saca el promedio de cada uno
  if(j == CANTIDAD_SENSADA && k == CANTIDAD_SENSADA) {
    float promedio1 = promedioSuma(dataSensada);
    float promedio2 = promedioSuma(dataSensada2);
    //Serial.println(promedio2 - promedio1);

    // Si la diferencia entre el promedio de los segundos valores leídos es mayor a 
    // la DIFERENCIA_MINIMA entonces quiere decir que hubo una variación (alguien hizo un soplido)
    // entonces apago y prendo el led y muestro la poesía
    if(promedio2 - promedio1 > DIFERENCIA_MINIMA){
        digitalWrite(PIN_LED, LOW);
        delay(2000);            
        digitalWrite(PIN_LED, HIGH);
        for(int i=0; i<random(3,6); i++) {
          mostrarPalabra();  
        }
        lcd.clear();
        lcd.noBacklight(); 
    }
    j = 0;
    k = 0;
  }

}

void mostrarPalabra() {

  String palabraRecortada1;
  String palabraRecortada2;
  int maxCaracteresPorRenglon = 8;
  
  // Obtiene una palabra al azar del array
  int posicion = random(CANTIDAD_PALABRAS);
  String palabra = palabras[posicion];
  Serial.println(posicion);
  Serial.println(palabra);
  int longitudPalabra = palabra.length();
   Serial.println(longitudPalabra);

  // La libreria del display de 16x1 lo maneja como si fuera 8x2
  // por eso tengo que cortar la palabra si tiene más de 8 caracteres 
  // y pasarla al "renglón" siguiente
  if(longitudPalabra > maxCaracteresPorRenglon) {   
    palabraRecortada1 = palabra.substring(0, 8);
    palabraRecortada2 = palabra.substring(8, longitudPalabra);
  } else {
    palabraRecortada1 = palabra;
  }

  // Limpio y prendo el display
  lcd.clear();
  lcd.backlight();
  // Posiciono el cursor del primer "renglón" e imprimo
  lcd.setCursor(0,0);
  lcd.print(palabraRecortada1); 
  // Posiciono el cursor del segundo "renglón" e imprimo
  lcd.setCursor(0,1);
  lcd.print(palabraRecortada2); 
  // Espero 3 segundos para poder leer la palabra
  delay(3000);
}

float promedioSuma(long int dataSensada[]) {
   
   long int sum = 0;
   
   for (int i = 0; i < CANTIDAD_SENSADA; i++) {
    sum = sum + dataSensada[i]; 
   }

   return sum/CANTIDAD_SENSADA;
}
