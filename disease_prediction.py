import numpy as np
import matplotlib.pyplot as plt

patients = []
diseases = []
MAX_PATIENTS = 50

NORMAL_DNA = "ATGCGTACGTTAGC"

risk_chart = [
    {"label": "No Risk", "min": 0, "max": 10, "precautions": "Maintain healthy lifestyle."},
    {"label": "Low Risk", "min": 11, "max": 30, "precautions": "Regular checkups, balanced diet."},
    {"label": "Moderate Risk", "min": 31, "max": 60, "precautions": "Consult doctor, monitor symptoms, specific diet."},
    {"label": "High Risk", "min": 61, "max": 100, "precautions": "Immediate medical attention, medication, strict isolation."}
]


def display_menu():
    print("\n=============================================")
    print("1) Patient Profile")
    print("2) Disease Profile")
    print("3) Reference Tables")
    print("4) Patient Records")
    print("5) Mutation Analysis")
    print("6) Disease Risk Prediction")
    print("7) Pandemic Spread Simulation")
    print("8) Exit")
    print("=============================================")


def patient_profile():
    if len(patients) >= MAX_PATIENTS:
        print("Error: Patient database is full!")
        return

    print("\n--- Add Patient Profile ---")
    name = input("Enter Name: ")
    age = input("Enter Age: ")
    dob = input("Enter Date of Birth (DD/MM/YYYY): ")
    blood_group = input("Enter Blood group (A+/B+/AB+/O+/A-/B-/AB-/O-): ")
    height = input("Enter Height (cm): ")
    weight = input("Enter Weight (kg): ")

    patients.append({
        "name": name,
        "age": age,
        "dob": dob,
        "blood_group": blood_group,
        "height": height,
        "weight": weight,
        "mutation_pct": 0.0,
        "risks": {}
    })
    print("Patient added successfully!")


def disease_profile():
    print("\n--- Add Disease Profile ---")
    name = input("Enter Disease Name: ")
    symptoms = input("Enter Symptoms: ")
    ref_code = input("Enter Reference Code: ")

    diseases.append({
        "name": name,
        "symptoms": symptoms,
        "code": ref_code
    })
    print("Disease added successfully!")


def reference_tables():
    while True:
        print("\n--- Reference Tables ---")
        print("1) Disease Chart")
        print("2) Risk Chart")
        choice = input("Enter a number: ")

        if choice == '1':
            print("\n--- Disease Chart ---")
            if not diseases:
                print("No diseases in the chart yet.")
            else:
                for d in diseases:
                    print(f"Name: {d['name']:<15} | Code: {d['code']:<5} | Symptoms: {d['symptoms']}")
            break
        elif choice == '2':
            print("\n--- Risk Chart ---")
            for r in risk_chart:
                print(f"Label: {r['label']:<15} | Range: {r['min']:>2}-{r['max']:<3}% | Precautions: {r['precautions']}")
            break
        else:
            print("Error: Invalid Input. Please enter 1 or 2.")


def patient_records():
    print("\n--- Patient Records ---")
    if not patients:
        print("No patient entries found.")
        return

    for p in patients:
        print(f"\nName: {p['name']:<15} | Age: {p['age']:<3} | Blood: {p['blood_group']:<3} | Height: {p['height']:<5} | Weight: {p['weight']:<5}")
        if p['risks']:
            print("Assigned Risk Labels:")
            for d_name, risk_data in p['risks'].items():
                print(f"  - {d_name}: {risk_data['label']} ({risk_data['percentage']:.2f}%)")
        else:
            print("Risks: No diseases checked yet.")


def get_patient():
    if not patients:
        print("No patients available.")
        return None
    for idx, p in enumerate(patients):
        print(f"{idx + 1}) {p['name']}")
    try:
        choice = int(input("Select patient number: ")) - 1
        if 0 <= choice < len(patients):
            return patients[choice]
        else:
            print("Invalid selection.")
            return None
    except ValueError:
        print("Error: Invalid Input")
        return None


def get_disease():
    if not diseases:
        print("No diseases available in the chart.")
        return None
    for idx, d in enumerate(diseases):
        print(f"{idx + 1}) {d['name']}")
    try:
        choice = int(input("Select disease number: ")) - 1
        if 0 <= choice < len(diseases):
            return diseases[choice]
        else:
            print("Invalid selection.")
            return None
    except ValueError:
        print("Error: Invalid Input")
        return None


def mutation_analysis():
    print("\n--- Mutation Analysis ---")
    p = get_patient()
    if not p: return

    print(f"\nReference DNA: {NORMAL_DNA}")
    input_dna = input("Enter DNA strand code for patient: ").upper()
    
    if len(input_dna) != len(NORMAL_DNA):
        print(f"Error: Entered DNA strand must be {len(NORMAL_DNA)} characters long.")
        return

    arr_normal = np.array(list(NORMAL_DNA))
    arr_input = np.array(list(input_dna))

    mutations = (arr_normal != arr_input)
    
    mutation_count = np.sum(mutations)
    mutation_pct = (mutation_count / len(NORMAL_DNA)) * 100
    p['mutation_pct'] = mutation_pct

    print(f"\nLevel of mutation present: {mutation_pct:.2f}%")

    if mutation_count > 0:
        print("Specific Mutation Details (Codon Analysis):")
        for i in range(0, len(NORMAL_DNA), 3):
            codon_normal = "".join(arr_normal[i:i+3])
            codon_input = "".join(arr_input[i:i+3])
            
            if codon_normal != codon_input:
                print(f"  - Codon Block {i//3 + 1}: Mutated to '{codon_input}', normally should be '{codon_normal}'")
    else:
        print("No mutations present.")


def disease_risk_prediction():
    print("\n--- Disease Risk Prediction ---")
    p = get_patient()
    if not p: return

    d = get_disease()
    if not d: return

    base_mutation_risk = p.get('mutation_pct', 0.0)

    susceptibility_factor = np.random.uniform(10.0, 40.0)
    total_risk = min(100.0, (base_mutation_risk * 1.5) + susceptibility_factor)

    assigned_label = "No Risk"
    for r in risk_chart:
        if r['min'] <= total_risk <= r['max']:
            assigned_label = r['label']
            break
    
    p['risks'][d['name']] = {
        'percentage': total_risk,
        'label': assigned_label
    }

    print(f"\n[Analysis Complete] Patient: {p['name']} | Disease: {d['name']}")
    print(f"Calculated Risk Level: {total_risk:.2f}%")
    print(f"Assigned Risk Label: {assigned_label}")


def pandemic_spread_simulation():
    print("\n--- Pandemic Spread Simulation ---")
    d = get_disease()
    if not d: return

    days_since_onset = int(np.random.randint(10, 100))
    total_population = 100000

    susceptible = int(np.random.randint(20000, 70000))
    recovered = int(np.random.randint(5000, 25000))
    infected = total_population - susceptible - recovered

    print(f"\nDisease name: {d['name']}")
    print(f"Number of days since onset: {days_since_onset}")
    print(f"Number of individuals susceptible: {susceptible}")
    print(f"Number of individuals infected: {infected}")
    print(f"Number of people recovered: {recovered}")

    print("Generating simulation pie chart display...")
    
    labels = ['Susceptible', 'Infected', 'Recovered']
    sizes = [susceptible, infected, recovered]
    colors = ['#8fc3a3', '#d9534f', '#5bc0de']
    
    plt.figure(figsize=(7, 7))
    plt.pie(sizes, labels=labels, colors=colors, autopct='%1.1f%%', startangle=140, explode=(0.05, 0.05, 0.05))
    plt.title(f"Pandemic Spread Simulation - {d['name']}")
    plt.axis('equal')
    
    plt.show()


def main():
    while True:
        display_menu()
        choice = input("Enter a number: ")

        if choice == '1':
            patient_profile()
        elif choice == '2':
            disease_profile()
        elif choice == '3':
            reference_tables()
        elif choice == '4':
            patient_records()
        elif choice == '5':
            mutation_analysis()
        elif choice == '6':
            disease_risk_prediction()
        elif choice == '7':
            pandemic_spread_simulation()
        elif choice == '8':
            print("Exiting...")
            break
        else:
            print("Error: Invalid Input")


if __name__ == "__main__":
    main()