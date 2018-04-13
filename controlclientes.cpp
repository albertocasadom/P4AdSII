#include <iostream>
#define MAX_CLIENTES 25
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

int alta_usr(){

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

	int i = 0;

	for(int j = 0; j<MAX_CLIENTES; j++){
		if(datos_cl[j].dni == dni){
			cout << "El usuario ya está dado de alta" << endl;
			return -1;
		}
	}

	datos_cl[user] = new_user;
	user++;

	return 0;
}

void imprimir_datos_cl(){

	cout << "   DNI   | Nombre  |	Tarifa 	 |  Alta  |Descuento " << endl;
	for(int i = 0; i<user;i++){
		cout << datos_cl[i].dni << " |" << datos_cl[i].nombre << " | " << datos_cl[i].tarifa << " | " 
		<< datos_cl[i].alta << " | " << datos_cl[i].descuento << endl;
	}
}

int main(){

	int alta = 0;

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
				break;
				case 4:
				break;
				case 5:
				break;
				case 6:
					exit(0);
				break;
		}
	}
}