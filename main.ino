#include <arduino.h>

int level = 1;

int display_leds[3] = {5,6,7};


int button_outputs[3] = {2,3,4};

int button_inputs[3] = {8,9,10};
int button_input_values[3] = {};


int green_light = 12;
int red_light = 11;

int input_value;

int pressed_sequence[12] = {};
int pressed_sequence_counter = 0;

int random_sequence[12] = {};


void setup (){
    Serial.begin(9600);

    delay(1000);

    for (int i = 0; i < 3; i++)
    {
        pinMode(button_outputs[i], OUTPUT);
        pinMode(button_inputs[i], INPUT);
        digitalWrite(button_outputs[i], HIGH);
    }
    
    pinMode(green_light, OUTPUT);
    pinMode(red_light, OUTPUT);

    start_level();
}


void start_level () {

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(green_light, HIGH);
        delay(200);
        digitalWrite(green_light, LOW);
        delay(200);
    }

    delay(1000);

    generate_random_sequence();
    digitalWrite(green_light, HIGH);
    listening();
}


void generate_random_sequence () {
    int random_index;

    for (int i = 0; i < (level + 2); i++)
    {
        randomSeed(analogRead(A0));
        random_index = random(0,3);

        random_sequence[i] = random_index;
        Serial.println(random_sequence[i]);

        digitalWrite(display_leds[random_index], HIGH);
        delay(750);
        digitalWrite(display_leds[random_index], LOW);
        delay(750);
    }
    
}


void verify_sequence() {

    for (int i = 0; i < 3; i++)
    {
        button_input_values[i] = -1;
    }
    


    pressed_sequence_counter = 0;
    for (int i = 0; i < (level + 2); i++)
    {
            Serial.print("Random: ");
            Serial.println(random_sequence[i]);

            Serial.println();

            Serial.print("Pressed: ");
            Serial.println(pressed_sequence[i]);

        if (random_sequence[i] != pressed_sequence[i])
        {
            return end_game();
        }
    }



    
    celebrate();
    level++;
    start_level();
    
}


void celebrate() {
    for (int i = 0; i < 7; i++)
    {
        digitalWrite(green_light, HIGH);
        delay(100);
        digitalWrite(green_light, LOW);
        delay(100);
    }
    
    Serial.print("Well Done You Just Completed Level: ");
    Serial.println(level);
}


void end_game(){
    level = 1;

    for (int i = 0; i < 7; i++)
    {
        digitalWrite(red_light, HIGH);
        delay(300);
        digitalWrite(red_light, LOW);
        delay(300);
    }
    

    start_level();
}



void listening () {
    while (true)
    {
        for (int i = 0; i < 3; i++)
        {
            button_input_values[i] = digitalRead(button_inputs[i]);
            
            if (button_input_values[i] == 1)
            {
                while (true)
                {
                    button_input_values[i] = digitalRead(button_inputs[i]);


                    if (button_input_values[i] == 0)
                    {
                            pressed_sequence_counter++;
                            pressed_sequence[(pressed_sequence_counter - 1)] = i;


                            //Serial.println(random_sequence[pressed_sequence_counter]);
                            //Serial.println(pressed_sequence[(pressed_sequence_counter - 1)]);
                            //Serial.println(pressed_sequence_counter);
                        
                        
                            if (pressed_sequence_counter == (level + 2))
                            {
                                Serial.print("Gay: ");
                                Serial.println(pressed_sequence_counter);
                                return verify_sequence();
                            }
                        break;
                    }
                    
                }
                
            }
            
        }
        
    }
    
}

void loop () {

}