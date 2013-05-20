//pins for the common anode rgb LED
const int rgbPins[] = {9,10,11};
double RED[] = {0, 1, 1};
double GREEN[] = {1, 0, 1};
double BLUE[] = {1, 1, 0};
double TEST[] = {1, 0, 0};
int MAX_ERRORS = 100;
/* Vars for the dial */
#define potPin A0
int currentLevel = 1;
int lastLR = 0;
int potLims[11][2] = {{500,510},{550,560},{600,610},{650,660},{700,710},{750,760},{800,810},{850,860},{900,910},{950,960},{1000,1024}};

/* Vars for button. */
int buttonPin=3;
boolean buttonState = false;
boolean resetNeeded = false;

/* Vars for serial comm. */
String inputString = "";
boolean inputComplete = false;
const int SERIAL_TIMEOUT = 100000;

void setup(){
  // Setup serial
  Serial.begin(9600);
  inputString.reserve(128);
  Serial.setTimeout(SERIAL_TIMEOUT);
  
  // setup reset button
  pinMode(buttonPin,INPUT);
  buttonState = digitalRead(buttonPin);
  
  // setup led
  for(int i=0; i<3; i++){
    pinMode(rgbPins[i],OUTPUT);
  }
  setColor(TEST);
  delay(1000);
  setColor(GREEN);
  
  pinMode(potPin,INPUT);
  currentLevel = getNearestLevel(analogRead(potPin));
  lastLR = currentLevel;
  //Serial.println("Current level :\t" + (String)currentLevel);
   
}

/* Do our soft reset routine. */
void myReset(){
  setColor(TEST);
  int code = 0;
  while(code!=100){
    requestReset();
    Serial.flush();
    char buf[3];
    Serial.readBytesUntil('\n',buf,3);
    code=atoi(buf);
    //Serial.println("Received code: " + (String)code); //debug
  }
  //Serial.println("reset ok"); //debug
  resetNeeded = false;
  setColor(GREEN);
}

/* Main loop. */
void loop(){
  /* Check button. */
  int bVal = digitalRead(buttonPin);
  //Serial.println("Button state: " + (String)bVal); // Debug
  if(bVal^buttonState){
    //Serial.println("Button state changed from: " + (String)buttonState + " -> " + (String)bVal); //debug
    buttonState = bVal;
    resetNeeded = !resetNeeded;
  }
  
  /* Reset only if the requested level and current level are equal
      i.e. last request was processed. */
  if(resetNeeded && lastLR==currentLevel){
    myReset();
  }
  
  /* Check pot. */
  int potVal = analogRead(potPin);
  //Serial.println("Pot value :\t" + (String)potVal); //Debug
  int nlevel = getLevel(potVal);
  if(!(nlevel==currentLevel || nlevel==lastLR || nlevel==0)){
    setColor(BLUE);
    requestLevelChange(nlevel);
    lastLR = nlevel;
  }
}

void setColor(double* color){
  for(int i=0; i<3; i++){
    analogWrite(rgbPins[i],*color * 255);
    color++;
  }
}

/* Sends a request for reset. */
void requestReset(){
  Serial.println("reset:1");
}
/* Sends request to change level. */
void requestLevelChange(int nl){
  Serial.println("dynos:" + (String)nl);
}

/* Maps the specified pot value to one of the dial levels. */
int getLevel(int p){
  for(int i=0; i<11; i++){
    if(p>=potLims[i][0] && p<=potLims[i][1]){
      return i+1;
    }
  }
  return 0;
}

/* Gets the nearest level to the pot value.
    This is useful when we need to have a nonzero level. */
int getNearestLevel(int p){
  int level;
  for(int i=0; i<11; i++){
    if(p<potLims[i][1]){
      level=i+1;
      break;
    }
  }
  return level;
}

/* Processes an input code. */
void processIn(String rs){
  int code = str2Int(rs);
  //Serial.println("recieved code: " + String(code)); //debug
  if(code==lastLR){
    currentLevel = lastLR;
    setColor(GREEN);
    return;
  }
  
  if(code>=1000){
    double val = 0.0;
    val = double(code-1000)/double(MAX_ERRORS);
    //Serial.println(val);//debug
    if(val<0.0){
      val=0.0;
    }
    else if(val>1.0){
      val=1.0;
    }
    double ncolor[3];
    ncolor[0] = 1.0-val;
    ncolor[1] = val;
    ncolor[2] = 1.0;
    setColor(ncolor);
  }
}
/* Serial event to receive data from Serial Port. */
void serialEvent() {
  while(Serial.available()){
    //get the new byte:
    char inChar = (char)Serial.read();
    
    //check if line is complete
    if(inChar=='\n'){
      inputComplete = true;
      processIn(inputString);
      inputString = "";
    }
    else{
      inputString += inChar;
    }
  }
}

// split a string into 3 doubles
double* splitCodes(String s){
  double ret[3];
  int len = s.length();
  int sidx = 0;
  int counter = 0;
  while(sidx<len){
    int eidx = sidx+1;
    while(s.charAt(eidx)!=',' && eidx<len){
      eidx++;
    }
    ret[counter] = str2Float(s.substring(sidx,eidx));
    sidx = eidx+1;
  }
  return ret;
}

double str2Float(String str){
  char carray[str.length() + 1]; //determine size of the array
  str.toCharArray(carray, sizeof(carray)); //put str into an array
  return atof(carray);
}

int str2Int(String str){
  char carray[str.length() + 1]; //determine size of the array
  str.toCharArray(carray, sizeof(carray)); //put str into an array
  return atoi(carray);
}
//cycle through all colors
void test(){
  double ss = 0.1;
  for(double r=1.0; r>0; r-=ss){
    for(double g=1.0; g>0; g-=ss){
      for(double b=1.0; b>0; b-=ss){
        TEST[0] = r;
        TEST[1] = g;
        TEST[2] = b;
        setColor(TEST);
        delay(50);
      }
    }
  } 
}