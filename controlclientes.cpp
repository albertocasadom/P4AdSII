#include <iostream>
#include <pthread.h> 
#include <unistd.h>
#define MAX_CLIENTES 25

using namespace std;

pthread_mutex_t mutex_datoscl = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t factura_cv = PTHREAD_COND_INITIALIZER;

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
 int change = 0;
 double facturacion;

int menu(){

	int option = 0;

	cout << endl;
	cout << "[1] Imprimir datos de clientes\n";
	cout << "[2] Alta de usuario\n";
	cout << "[3] Baja de usuario\n";
	cout << "[4] Cambio de tarifa\n";
	cout << "[5] Actualizar descuentos\n";
	cout << "[6] Terminar\n\n";
	cout << "Opción: ";
	cin >> option;

	return option;
}

void imprimir_datos_cl(){

	if(user == 0){
		cout << endl << " No hay clientes registrados" << endl << endl;
	}else{
		cout << "Listado de clientes: " << endl;
		pthread_mutex_lock(&mutex_datoscl);
		for(int i = 0; i<user;i++){
			cout << i+1 << ": " << datos_cl[i].dni << " | " << datos_cl[i].nombre << " | " << datos_cl[i].tarifa 
			<< " | " << datos_cl[i].alta << " | " << datos_cl[i].descuento << endl;
		}
		pthread_mutex_unlock(&mutex_datoscl);
	}
}

int alta_usr(){

	if (user < MAX_CLIENTES){ 
		unsigned dni, alta, descuento;
		string nombre;
		char tarifa;

		info_cliente_t new_user;

		cout << "DNI del usuario: ";
		cin >> new_user.dni;
		pthread_mutex_lock(&mutex_datoscl);
		for(int j = 0; j<user; j++){
			if(datos_cl[j].dni == new_user.dni){
				cout << "El usuario ya está dado de alta" << endl << endl;
				pthread_mutex_unlock(&mutex_datoscl);
				return -1;
			}
		}
		pthread_mutex_unlock(&mutex_datoscl);
		cout << "Nombre del usuario: ";
		cin >> new_user.nombre;
		cout << "Tarifa inicial: ";
		cin >> new_user.tarifa;
		cout << "Fecha de alta: ";
		cin >> new_user.alta;
		cout << "Descuento inicial: ";
		cin >> new_user.descuento;
		cout << endl;

		pthread_mutex_lock(&mutex_datoscl);

		datos_cl[user] = new_user;
		user++;

		pthread_mutex_unlock(&mutex_datoscl);

		return 0;

	}else{
		cout << endl << "Se ha llegado al número máximo de clientes" << endl;
		return -1;
	}
}

unsigned baja_usr(unsigned dni){

	int i = 0;
	unsigned ok = 0;

	pthread_mutex_lock(&mutex_datoscl);
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
	pthread_mutex_unlock(&mutex_datoscl);

	return ok;
}
unsigned cambiar_tarifa(unsigned dni, char tarifa){

	unsigned ok = 0;
	pthread_mutex_lock(&mutex_datoscl);
	for(int i = 0; i< user; i++){
		if(datos_cl[i].dni == dni){
			datos_cl[i].tarifa = tarifa;
			ok = 1;
		}
	}
	pthread_mutex_unlock(&mutex_datoscl);
	return ok;
}

void * actualizar_desc(void * var){

	long secs = (long) var;
	while(1){
		sleep(secs);
		pthread_mutex_lock (&mutex_datoscl);
		for(int i = 0; i< user; i++){
			if((datos_cl[i].tarifa == 'A') && (datos_cl[i].alta <= 2008)){
				if(datos_cl[i].descuento != 30){
					datos_cl[i].descuento = 30;
					change = 1;
				}
			}else if((datos_cl[i].tarifa == 'A') && (datos_cl[i].alta > 2008 && datos_cl[i].alta <=2012)){
					if(datos_cl[i].descuento != 40){
						datos_cl[i].descuento = 40;
						change = 1;
					}
			}else if((datos_cl[i].tarifa == 'A') && (datos_cl[i].alta > 2012)){
					if(datos_cl[i].descuento != 25){						
						datos_cl[i].descuento = 25;
						change = 1;
					}	
			}else{
				if(datos_cl[i].descuento != 0){
				datos_cl[i].descuento = 0;
				change = 1;
				}	
			}
		}
		pthread_mutex_unlock(&mutex_datoscl);
		pthread_mutex_lock (&mutex_datoscl);
		if(change == 1){
			pthread_cond_signal(&factura_cv);
		}
		pthread_mutex_unlock(&mutex_datoscl);
	}
	
}

void * print_tarifa(void * var){
	
	pthread_mutex_lock(&mutex_datoscl);
	while(change == 0){
		pthread_cond_wait(&factura_cv, &mutex_datoscl);
			if(change == 1){
				facturacion = 0;
				for(int i = 0; i< user; i++){
					if(datos_cl[i].tarifa == 'A'){
							facturacion = facturacion + (800 * (1 -(datos_cl[i].descuento * 0.01)));
					}else if(datos_cl[i].tarifa == 'B'){
							facturacion = facturacion + (600 * (1 -(datos_cl[i].descuento * 0.01)));
					}else if(datos_cl[i].tarifa == 'C'){
							facturacion = facturacion + (300 * (1 -(datos_cl[i].descuento * 0.01)));
					}else{
						cout << "La tarifa no es correcta, por favor revise los datos del usuario: " << datos_cl[i].dni << endl;
					}
				}	
			}		
		change = 0;
	}
	pthread_mutex_unlock(&mutex_datoscl);
}

void terminar(pthread_t h_desc, pthread_t h_factura){
	pthread_cancel(h_desc);
	pthread_cancel(h_factura);
	exit(0);
}	


int main(){

	int alta = 0;
	unsigned dni;
	unsigned ok;
	char tarifa;
	long secs;
	pthread_t h_desc, h_factura;
	int ret = 0;

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
					cout << "Introduzca la nueva tarifa: ";
					cin >> tarifa;
					if(tarifa == 'A' || tarifa == 'B' || tarifa == 'C'){
						ok = cambiar_tarifa(dni, tarifa);
						if(ok == 0){
							cout << "No existe ningún usuario con el DNI introducido." << endl;
						}
					
					}else{
						cout << "Introduzca una tarifa valida: A, B o C" << endl;
					}
					
				break;
				case 5:
					cout << "Introduzca la periodicidad con la que desee actualizar los descuentos: ";
					cin >> secs;
					ret = pthread_create(&h_desc, NULL,actualizar_desc, (void *)secs);
					if(ret){
						cout << "Error en la creación de la hebra: " << ret << endl;
						exit(-1);
					}
					ret = pthread_create(&h_factura, NULL, print_tarifa, NULL);
					if(ret){
						cout << "Error en la creación de la hebra: " << ret << endl;
						exit(-1);
					}
				break;
				case 6:
					terminar(h_desc,h_factura);
				break;
		}
	}
pthread_exit(NULL);
}