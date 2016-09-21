/*
 *
 *
 *      Author: Kameron Kranse, Kasy Treu, Abdulaziz Alsaleh
 */
#include <stdio.h>

void gpsScan();

int main(){
    gpsScan();
    return 0;
}

void gpsScan(){
    FILE *file;
    file = fopen("GPS_characters.txt","r"); //open GPS text file
    char c;
    char dataSet[80];
    int calculatedChecksum = 0;
    
    int checkSumFromFIle;
    char checksum1;
    char checksum2;
    char str[5];
    int read_status;
    
    read_status = fscanf(file, "%c", &c);
    while(read_status == 1){
        if(c == '$'){
            read_status = fscanf(file, "%c", &c);
            if(c == 'G'){
                dataSet[0] = c;     //if the first character in the line after the $ is a G, store to array
                read_status = fscanf(file, "%c", &c);
                if(c == 'P'){
                    dataSet[1] = c;//if the first character in the line after G is P, store to next array spot
                    read_status = fscanf(file, "%c", &c);
                    if(c == 'G'){
                        dataSet[2] = c;//if the first character in the line after P is G, store to next array spot
                        read_status = fscanf(file, "%c", &c);
                        if(c == 'G'){
                            dataSet[3] = c;//if the first character in the line after G is G, store to next array spot
                            read_status = fscanf(file, "%c", &c);
                            if(c == 'A'){
                                dataSet[4] = c;//if the first character in the line after G is A, store to next array spot
                                
                                int i = 5;
                                while(c != '*'){
                                read_status = fscanf(file, "%c", &c);// put all values in a char[] to XOR
                                    dataSet[i] = c;
                                    i++;
                                }
                                
                                int k;
                                for(k = 0; k < i-1; k++){
                                    calculatedChecksum ^= dataSet[k];
                                }
                                
                                read_status = fscanf(file, "%c", &c);
                                checksum1 = c;
                                
                                read_status = fscanf(file, "%c", &c);
                                checksum2 = c;
                                
                                str[0] = '0';                    // build a string out of the read in hex value
                                str[1] = 'x';
                                str[2] = checksum1;
                                str[3] = checksum2;
                                str[4] = '\0';
                                sscanf(str, "%x", &checkSumFromFIle);        // then convert it to an int
                                
                                if(calculatedChecksum == checkSumFromFIle){
									//print time
                                    printf("TIME: %c%c hours %c%c min %c%c%c%c%c sec\n", dataSet[6], dataSet[7], dataSet[8], dataSet[9], dataSet[10], dataSet[11],
                                           dataSet[12], dataSet[13], dataSet[14]);
									//print latitude
                                    printf("Latitude: %c%c Deg %c%c%c%c%c%c%c%c min %c\n", dataSet[16],dataSet[17], dataSet[18], dataSet[19], dataSet[20],
                                           dataSet[21], dataSet[22], dataSet[23], dataSet[24], dataSet[25], dataSet[27]);
									//print longitude
                                    printf("Longitude: %c%c%c Deg %c%c%c%c%c%c%c%c min %c\n", dataSet[29], dataSet[30], dataSet[31], dataSet[32], dataSet[33],
                                           dataSet[34], dataSet[35], dataSet[36], dataSet[37], dataSet[38], dataSet[39], dataSet[41]);
									//print elevation
                                    printf("Height: %c%c%c%c%c%c %c\n", dataSet[53], dataSet[54], dataSet[55], dataSet[56], dataSet[57], dataSet[58], dataSet[60]);
                                }
                                read_status = fscanf(file, "%c", &c);
                                
                            }
                            
                        }
                        
                        else if(c == 'S'){
                            dataSet[3] = c;
                            read_status = fscanf(file, "%c", &c);
                            if(c == 'A'){
                                dataSet[4] = c;
                                int l = 5;
                                while(c != '*'){
                                    read_status = fscanf(file, "%c", &c);
                                    dataSet[l] = c;
                                    l++;
                                }
                                int m;
                                calculatedChecksum = 0;
                                for(m = 0; m < l-1; m++){
                                    calculatedChecksum ^= dataSet[m];
                                }
                                
                                read_status = fscanf(file, "%c", &c);
                                checksum1 = c;
                                
                                read_status = fscanf(file, "%c", &c);
                                checksum2 = c;
                                
                                str[0] = '0';           // build a string out of the read in hex value
                                str[1] = 'x';
                                str[2] = checksum1;
                                str[3] = checksum2;
                                str[4] = '\0';
                                
                                sscanf(str, "%x", &checkSumFromFIle);        // then convert to an int
                                
                                if(calculatedChecksum == checkSumFromFIle){
                                    printf("Numbers of satellites: ");
                                    int comas = 0;
                                    int place = 8;
                                    while(comas < 8){
                                        printf("%c", dataSet[place]);// always 8 commas
                                        if(dataSet[place] == ','){
                                            comas ++;
                                        }
                                        place++;
                                    }
                                }
                                
                                
                            }
                        }
                        
                    }
                    
                }
                
            }
            
        }
        else{
            read_status = fscanf(file, "%c", &c);          /* if the char isn't useful, move on */
        }
    }
}
