#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PATIENTS 50
#define MAX_DISEASES 50
#define DNA_LENGTH 14

struct DiseaseRisk {
    char diseaseName[50];
    float percentage;
    char label[30];
};

struct Patient {
    char name[50];
    int age;
    char dob[15];
    char bloodGroup[5];
    float height;
    float weight;
    float mutation_pct;
    struct DiseaseRisk risks[MAX_DISEASES];
    int riskCount;
};

struct Disease {
    char name[50];
    char symptoms[100];
    char code[10];
};

struct RiskLevel {
    char label[30];
    int min;
    int max;
    char precautions[100];
};

struct Patient patients[MAX_PATIENTS];
int patientCount = 0;

struct Disease diseases[MAX_DISEASES];
int diseaseCount = 0;

const char NORMAL_DNA[] = "ATGCGTACGTTAGC";

struct RiskLevel riskChart[] = {
    {"No Risk", 0, 10, "Maintain healthy lifestyle."},
    {"Low Risk", 11, 30, "Regular checkups, balanced diet."},
    {"Moderate Risk", 31, 60, "Consult doctor, monitor symptoms, specific diet."},
    {"High Risk", 61, 100, "Immediate medical attention, medication."}
};
int numRiskLevels = 4;

void patientProfile() {
    if (patientCount >= MAX_PATIENTS) {
        printf("Error: Patient database is full!\n");
        return;
    }
    printf("\n--- Add Patient Profile ---\n");
    printf("Enter Name: ");
    scanf(" %[^\n]", patients[patientCount].name);
    printf("Enter Age: ");
    scanf("%d", &patients[patientCount].age);
    printf("Enter Date of birth (DD/MM/YYYY): ");
    scanf(" %s", patients[patientCount].dob);
    printf("Enter Blood group (A+/B+/AB+/O+/A-/B-/AB-/O-): ");
    scanf(" %s", patients[patientCount].bloodGroup);
    printf("Enter Height (cm): ");
    scanf("%f", &patients[patientCount].height);
    printf("Enter Weight (kg): ");
    scanf("%f", &patients[patientCount].weight);
    
    patients[patientCount].mutation_pct = 0.0;
    patients[patientCount].riskCount = 0;
    
    patientCount++;
    printf("Patient added successfully!\n");
}

void diseaseProfile() {
    if (diseaseCount >= MAX_DISEASES) {
        printf("Error: Disease database is full!\n");
        return;
    }
    printf("\n--- Add Disease Profile ---\n");
    printf("Enter Disease Name: ");
    scanf(" %[^\n]", diseases[diseaseCount].name);
    printf("Enter Symptoms: ");
    scanf(" %[^\n]", diseases[diseaseCount].symptoms);
    printf("Enter Reference Code: ");
    scanf(" %s", diseases[diseaseCount].code);
    
    diseaseCount++;
    printf("Disease added successfully!\n");
}

void referenceTables() {
    int choice;
    printf("\n--- Reference Tables ---\n");
    printf("1) Disease Chart\n");
    printf("2) Risk Chart\n");
    printf("Enter a number: ");
    
    if (scanf("%d", &choice) != 1) {
        while(getchar() != '\n');
        printf("Error: Invalid Input\n");
        return;
    }
    
    if (choice == 1) {
        printf("\n--- Disease Chart ---\n");
        if (diseaseCount == 0) {
            printf("No diseases in the chart yet.\n");
        } else {
            for (int i = 0; i < diseaseCount; i++) {
                printf("Name: %-15s | Code: %-5s | Symptoms: %s\n", diseases[i].name, diseases[i].code, diseases[i].symptoms);
            }
        }
    } else if (choice == 2) {
        printf("\n--- Risk Chart ---\n");
        for (int i = 0; i < numRiskLevels; i++) {
            printf("Label: %-15s | Range: %2d-%-3d%% | Precautions: %s\n", riskChart[i].label, riskChart[i].min, riskChart[i].max, riskChart[i].precautions);
        }
    } else {
        printf("Error: Invalid Input\n");
    }
}

void patientRecords() {
    printf("\n--- Patient Records ---\n");
    if (patientCount == 0) {
        printf("No patient entries found.\n");
        return;
    }
    for (int i = 0; i < patientCount; i++) {
        printf("\nName: %-15s | Age: %-3d | Blood: %-3s | Height: %-5.1f | Weight: %-5.1f\n", 
               patients[i].name, patients[i].age, patients[i].bloodGroup, patients[i].height, patients[i].weight);
        
        if (patients[i].riskCount > 0) {
            printf("Assigned Risk Labels:\n");
            for (int j = 0; j < patients[i].riskCount; j++) {
                printf("  - %s: %s (%.2f%%)\n", patients[i].risks[j].diseaseName, patients[i].risks[j].label, patients[i].risks[j].percentage);
            }
        } else {
            printf("Risks: No diseases checked yet.\n");
        }
    }
}

int selectPatient() {
    if (patientCount == 0) {
        printf("No patients available.\n");
        return -1;
    }
    for (int i = 0; i < patientCount; i++) {
        printf("%d) %s\n", i + 1, patients[i].name);
    }
    int choice;
    printf("Select patient number: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > patientCount) {
        while(getchar() != '\n'); 
        printf("Error: Invalid Input\n");
        return -1;
    }
    return choice - 1;
}

int selectDisease() {
    if (diseaseCount == 0) {
        printf("No diseases available in the chart.\n");
        return -1;
    }
    for (int i = 0; i < diseaseCount; i++) {
        printf("%d) %s\n", i + 1, diseases[i].name);
    }
    int choice;
    printf("Select disease number: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > diseaseCount) {
        while(getchar() != '\n');
        printf("Error: Invalid Input\n");
        return -1;
    }
    return choice - 1;
}

void mutationAnalysis() {
    printf("\n--- Mutation Analysis ---\n");
    int pIdx = selectPatient();
    if (pIdx == -1) return;
    
    printf("\nReference DNA: %s\n", NORMAL_DNA);
    printf("Enter DNA strand code for patient: ");
    char inputDNA[50];
    scanf(" %s", inputDNA);
    
    if (strlen(inputDNA) != DNA_LENGTH) {
        printf("Error: Entered DNA strand must be exactly %d characters long.\n", DNA_LENGTH);
        return;
    }
    
    int diffs = 0;
    for (int i = 0; i < DNA_LENGTH; i++) {
        if (inputDNA[i] != NORMAL_DNA[i]) {
            diffs++;
        }
    }
    
    float pct = ((float)diffs / DNA_LENGTH) * 100.0f;
    patients[pIdx].mutation_pct = pct;
    
    printf("\nLevel of mutation present: %.2f%%\n", pct);
    
    if (diffs > 0) {
        printf("Specific Mutation Details (Codon Analysis):\n");
        for (int i = 0; i < DNA_LENGTH; i += 3) {
            char cNorm[4] = {0}, cIn[4] = {0};
            strncpy(cNorm, &NORMAL_DNA[i], 3);
            strncpy(cIn, &inputDNA[i], 3);
            
            if (strcmp(cNorm, cIn) != 0) {
                printf("  - Block %d: Mutated to '%s', normally should be '%s'\n", (i/3)+1, cIn, cNorm);
            }
        }
    } else {
        printf("No mutations detected.\n");
    }
}

void diseaseRiskPrediction() {
    printf("\n--- Disease Risk Prediction ---\n");
    int pIdx = selectPatient();
    if (pIdx == -1) return;
    
    int dIdx = selectDisease();
    if (dIdx == -1) return;
    
    float baseRisk = patients[pIdx].mutation_pct * 1.5;
    float environmental = (rand() % 30) + 10.0;
    float totalRisk = baseRisk + environmental;
    if (totalRisk > 100.0) totalRisk = 100.0;
    
    char assignedLabel[30] = "No Risk";
    for (int i = 0; i < numRiskLevels; i++) {
        if (totalRisk >= riskChart[i].min && totalRisk <= riskChart[i].max) {
            strcpy(assignedLabel, riskChart[i].label);
            break;
        }
    }
    
    int rIdx = patients[pIdx].riskCount;
    if (rIdx < MAX_DISEASES) {
        strcpy(patients[pIdx].risks[rIdx].diseaseName, diseases[dIdx].name);
        patients[pIdx].risks[rIdx].percentage = totalRisk;
        strcpy(patients[pIdx].risks[rIdx].label, assignedLabel);
        patients[pIdx].riskCount++;
    }
    
    printf("\n[Analysis Complete] Patient: %s | Disease: %s\n", patients[pIdx].name, diseases[dIdx].name);
    printf("Calculated Risk Level: %.2f%%\n", totalRisk);
    printf("Assigned Risk Label: %s\n", assignedLabel);
}

void drawAsciiDemographicsChart(int sus, int inf, int rec) {
    int total = sus + inf + rec;
    if (total == 0) return;
    
    int barWidth = 40;
    int susLen = (int)(((float)sus / total) * barWidth);
    int infLen = (int)(((float)inf / total) * barWidth);
    int recLen = barWidth - susLen - infLen;
    
    printf("\n--- Demographic Chart (Terminal ASCII Block Representation) ---\n");
    printf("Susceptible | ");
    for(int i=0; i<susLen; i++) printf("\xB1");
    printf(" [%.1f%%]\n", ((float)sus/total)*100);
    
    printf("Infected    | ");
    for(int i=0; i<infLen; i++) printf("\xDB");
    printf(" [%.1f%%]\n", ((float)inf/total)*100);
    
    printf("Recovered   | ");
    for(int i=0; i<recLen; i++) printf("\xB0");
    printf(" [%.1f%%]\n", ((float)rec/total)*100);
    printf("-------------------------------------------------------------\n");
}

void pandemicSpreadSimulation() {
    printf("\n--- Pandemic Spread Simulation ---\n");
    int dIdx = selectDisease();
    if (dIdx == -1) return;
    
    int days = (rand() % 90) + 10;
    int totalPop = 100000;
    int sus = (rand() % 50000) + 20000;
    int rec = (rand() % 20000) + 5000;
    int inf = totalPop - sus - rec;
    
    printf("\nDisease name: %s\n", diseases[dIdx].name);
    printf("Number of days since onset: %d\n", days);
    printf("Number of individuals susceptible: %d\n", sus);
    printf("Number of individuals infected: %d\n", inf);
    printf("Number of people recovered: %d\n", rec);
    
    drawAsciiDemographicsChart(sus, inf, rec);
}

int main() {
    int choice;
    srand(time(NULL));
    
    while(1) {
        printf("\n=============================================\n");
        printf("1) Patient Profile\n");
        printf("2) Disease Profile\n");
        printf("3) Reference Tables\n");
        printf("4) Patient Records\n");
        printf("5) Mutation Analysis\n");
        printf("6) Disease Risk Prediction\n");
        printf("7) Pandemic Spread Simulation\n");
        printf("8) Exit\n");
        printf("=============================================\n");
        printf("Enter a number: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            printf("Error: Invalid Input\n");
            continue;
        }
        
        switch(choice) {
            case 1: patientProfile(); break;
            case 2: diseaseProfile(); break;
            case 3: referenceTables(); break;
            case 4: patientRecords(); break;
            case 5: mutationAnalysis(); break;
            case 6: diseaseRiskPrediction(); break;
            case 7: pandemicSpreadSimulation(); break;
            case 8: printf("Exiting...\n"); return 0;
            default: printf("Error: Invalid Input\n");
        }
    }
    return 0;
}