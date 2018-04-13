#include <iostream>
#define MAX_CLIENTES 3
#define T_A 1
#define T_B 2
#define T_C 3

using namespace std;

 struct info_cliente{
 unsigned dni;
 string nombre;
 char tarifa;
 unsigned alta;
 unsigned descuento;
 };

 typedef struct info_cliente info_cliente_t;

 info_cliente_t datos_cl [MAX_CLIENTES];
 static int user = 0;

int menu(){

	int option = 0;

	cout << endl;
	cout << "[1] Imprimir datos de clientes\n";
	cout << "[2] Alta de usuario\n";
	cout << "[3] Baja de usuario\n";
	cout << "[4] Cambio de tarifa\n";
	cout << "[5] Actualizar descuentos\n";
	cout << "[6] Terminar\n\n";
	cout << "Opción:";
	cin >> option;

	return option;
}

void imprimir_datos_cl(){

	if(user == 0){
		cout << endl << " No hay clientes registrados" << endl << endl;
	}else{
		for(int i = 0; i<user;i++){
			cout << "[" << i+1 << "] DNI: " << datos_cl[i].dni << " | Nombre: " << datos_cl[i].nombre 
			<< " | Tarifa: " << datos_cl[i].tarifa << " | Año de alta: " << datos_cl[i].alta << " | Descuento: "
			<< datos_cl[i].descuento << endl;
		}
	}
}

int alta_usr(){

	if (user < MAX_CLIENTES){ 
		unsigned dni, alta, descuento;
		string nombre;
		char tarifa;

		info_cliente_t new_user;

		cout << "Introduzca DNI del cliente: ";
		cin >> new_user.dni;
		cout << "Introduzca nombre del cliente: ";
		cin >> new_user.nombre;
		cout << "Introduzca tarifa (A, B o C) del cliente: ";
		cin >> new_user.tarifa;
		cout << "Introduzca año de alta del cliente: ";
		cin >> new_user.alta;
		cout << "Introduzca descuento del cliente: ";
		cin >> new_user.descuento;
		cout << endl;

		for(int j = 0; j<MAX_CLIENTES; j++){
			if(datos_cl[j].dni == new_user.dni){
				cout << "El usuario ya está dado de alta" << endl << endl;
				return -1;
			}
		}

		datos_cl[user] = new_user;
		user++;

		return 0;

	}else{
		cout << endl << "Se ha llegado al número máximo de clientes" << endl;
		return -1;
	}
}

unsigned baja_usr(unsigned dni){

	int i = 0;
	unsigned ok = 0;
	while(i < user){
		if(datos_cl[i].dni != dni){
			i++;
		}else{
			for(int x = i; x<user; x++){
				datos_cl[x] = datos_cl[x+1];
				user--;
				ok = 1;
			}
		}
	}

	return ok;
}
unsigned cambiar_tarifa(unsigned dni, char tarifa){

	unsigned ok = 0;

	for(int i = 0; i< user; i++){
		if(datos_cl[i].dni == dni){
			datos_cl[i].tarifa = tarifa;
			ok = 1;
		}
	}

	return ok;
}

int main(){

	int alta = 0;
	unsigned dni;
	unsigned ok;
	char tarifa;

		while (1){

			int option = menu();

			switch(option){
				case 1:
					imprimir_datos_cl();
				break;
				case 2:
					alta = alta_usr();
					if(alta != 0){
						cout << "No se puede dar de alta al usuario\n";
					}
				break;
				case 3:
					cout << "Introduzca el DNI del cliente que desee dar de baja: ";
					cin >> dni;
					ok = baja_usr(dni);
					if(ok == 0){
						cout << endl << "No existe ningún usuario con el DNI introducido." << endl;
					}
				break;
				case 4:
					cout << "Introduzca el DNI del cliente al que se le desea aplicar la nueva tarifa: ";
					cin >> dni;
					cout << endl << "Introduzca la nueva tarifa: ";
					cin >> tarifa;
					ok = cambiar_tarifa(dni, tarifa);
					if(ok == 0){
						cout << "No existe ningún usuario con el DNI introducido." << endl;
					}
				break;
				case 5:
				break;
				case 6:
					exit(0);
				break;
		}
	}
}