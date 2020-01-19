// ***** System.h *****

#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <iostream>

struct model
{   // per leggere il nome del modello e l'id
    int model_id;                                       // [model1.dat] (esempio)
    std::string model_name;
};

struct component
{   // per leggere le informazioni sui componenti nei file dei modelli
    int component_id;                                   // [model1.dat] (esempio)
    std::string component_name;
    int quantity;
};

struct component_info
{   // per leggere le informazioni dei componenti su components_info.dat
    int component_id;                                   // components_info.dat
    std::string component_name;
    double delivery_time;
};

struct order
{   // per leggere gli ordini su orders.dat
    int time_stamp;                                     // orders.dat
    int model_id;
    int quantity_ordered;
};

struct componentistica
{   // una volta presa visione degli ordini, questi sono i componenti totali per produrre un dato elettrodomestico
    int quantita_da_produrre;                           // data da (order.quantity_ordered * models_infos.quantity)
    int componente;                                     // dato da models_infos.component_id
    int mesi_attesa;                                    // dati da components_infos.delivery_time
};

class elettrodomestico
{   // e questo è il mio elettrodomestico nelle varie fasi della produzione

public:
    int model_id;                                       // dato da model.model_id
    int quantita_modello;                               // data da order.quantity_ordered
    std::vector<componentistica> componenti;            // vettore di struct componentistica
};

class System
{   // questa è la mia azienda!
    int month = -1;                                     // inizio da -1: alla prima operazione, month viene incrementato a 0

public:

    std::vector<model> model_types; 					// vettore con model_id, model_name
    std::vector<std::vector<component>> models_infos;   // vettori con component_id, component_name, quantity dentro un vettore
    std::vector<component_info> components_infos; 		// vettore con component_id, component_name, delivery_time
    std::vector<order> orders;                          // per salvare gli ordini da orders.dat
    
    // questo sarebbe il mio magazzino, con due scompartimenti, e uno scompartimento temporaneo
    std::vector<std::vector<elettrodomestico>> month_orders;           // per salvare gli elettrodomestici i cui componenti sono in arrivo
    std::vector<std::vector<elettrodomestico>> production_orders;      // per salvare gli elettrodomestici da produrre (quelli per cui bisogna aspettare ancora un mese)
    std::vector<componentistica> vettore_temporaneo;                   // mi serve solo per la funzione print
    
    
    void check_situation();                             // incrementa il mese (invocata una volta al mese), chiama la funzione che ordina componenti per quel mese e print() nel caso in cui un ordine sia evaso
    void print(); 						                // printa componenti in arrivo, componenti in uso, ordini evasi
    void fill_data(); 					                // invocata solo all'inizio per riempire i vettori con i dati utili
    void order_components(); 			                // inserisce i componenti da ordinare nel vettore month_orders
};

class Invalid {};   // per lanciare eccezioni

#endif
