// ***** System.cpp *****

#include "System.h"
#include <fstream>

void System::check_situation()
{   // incremento il mese, tolgo un mese ai componenti che aspetto, e ordino quelli del mese nuovo (se ho ordinazioni in quel mese)
	month++; // incrementa il mese come prima cosa

    // guardo la situazione in magazzino:
    // per quanto riguarda i componenti in attesa:
	if (month_orders.size() != 0)
	{
	    for (auto& vect : month_orders)  // per ogni vettore
        {
            std::vector<elettrodomestico> tmp;
            for (auto& x : vect)  // per ogni elettrodomestico x
            {
                int contatore = 0;
                for (auto& elem : x.componenti)  // per ogni elemento nel vettore delle componenti (cioè per ogni componente)
                {
                    if (elem.mesi_attesa == 0)
                    {
                        vettore_temporaneo.push_back(elem);  // mi serve per il print()
                        if (contatore == x.componenti.size()-1)   // se sono tutti pronti
                        {
                            tmp.push_back(x);   // mi salvo l'elettrodomestico pronto per la produzione sul vettore tmp
                            elem.mesi_attesa = -1;  // vuol dire che non lo voglio più considerare in month_orders
                        }
                        contatore++;  // vado avanti a contare, quando tutti i componenti avranno da attendere 0 mesi, il contatore sarà il numero dei componenti
                    }
                    elem.mesi_attesa--;
                }
            }
            production_orders.push_back(tmp);
        }
    }
    
    // e per quanto riguarda i componenti che sono pronti per essere assemblati (bisogna aspettare ancora un mese)
    if (production_orders.size() != 0)  // qui ci va il print
    {
        for (auto vect : production_orders)
        {
            for (auto x : vect)
            {
                print();
            }
        }
    }
    /*
    // print production_orders
    std::cout << "Dentro production_orders nel mese: " << month << std::endl;       // ***** debug *****
    for (auto vect : production_orders)                                             // ***** debug *****
    {                                                                               // ***** debug *****
        for (auto x : vect)                                                         // ***** debug *****
        {                                                                           // ***** debug *****
            std::cout << x.model_id << ", "                                         // ***** debug *****
                << x.quantita_modello << "\n"                                       // ***** debug *****
                << "Le componenti per questo modello sono:\n";                      // ***** debug *****
            for (auto elem : x.componenti)                                          // ***** debug *****
            {                                                                       // ***** debug *****
            std::cout << elem.quantita_da_produrre << ", "                          // ***** debug *****
                << elem.componente << ", "                                          // ***** debug *****
                << elem.mesi_attesa << std::endl;                                   // ***** debug *****
            }                                                                       // ***** debug *****
        }                                                                           // ***** debug *****
        std::cout << std::endl;                                                     // ***** debug *****
    }                                                                               // ***** debug *****
    */
    order_components(); // se ci sono ordini in quel mese (controlla già lui il fatto che siano solo di quel mese)
        
}

void System::print()
{
    // ...
}

void System::fill_data()
{
	//(1) riempio model_types e models_infos
	std::ifstream ist {"models.dat"};
    if (!ist)
		throw Invalid();

    // store models
    std::vector<std::string> model_files;                   // questo è il mio vettore con i model
    std::string file;
    
	while (ist >> file)
        model_files.push_back(std::string{file});

	ist.close();    // chiudo ist
/*
	// print models
	std::cout << "Dentro model_files:\n";                                           // ***** debug *****
    for (int i=0; i < model_files.size(); ++i)                                      // ***** debug *****
        std::cout << model_files[i] << std::endl;                                   // ***** debug *****
    std::cout << std::endl;                                                         // ***** debug *****
*/
    // store model_id, model_name, component_id, component_name, quantity
    for (int i=0; i < model_files.size(); ++i)
    {
        // input [model1.dat] (esempio)
        std::string input_name = model_files[i];
        std::ifstream ist2 {input_name};
        if (!ist2)
			throw Invalid();

        // store prima riga di [model1.dat] (esempio)
        int model_id;
        std::string model_name;
        
        ist2 >> model_id >> model_name;
        model_types.push_back(model{model_id, model_name}); // store prima riga

        // store seconda riga di [model1.dat] (esempio)
		int component_id;
        std::string component_name;
        int quantity;
		std::vector<component> tmp;

        while (ist2 >> component_id >> component_name >> quantity)
            tmp.push_back(component{component_id, component_name, quantity});   // store tutto il resto

		models_infos.push_back(tmp);
    }
/*
    // print model_id e model_name
    std::cout << "Dentro model_types:\n";                                           // ***** debug *****
    for (int j=0; j < model_types.size(); ++j)                                      // ***** debug *****
    {                                                                               // ***** debug *****
        std::cout << model_types[j].model_id << ", "                                // ***** debug *****
            << model_types[j].model_name                                            // ***** debug *****
            << std::endl;                                                           // ***** debug *****
    }                                                                               // ***** debug *****
    std::cout << std::endl;                                                         // ***** debug *****
*/
/*
    // print component_id, component_name e quantity
	std::cout << "Dentro i vettori dentro models_infos:\n";                         // ***** debug *****
    for (auto vec : models_infos)                                                   // ***** debug *****
    {                                                                               // ***** debug *****
        for (auto v : vec)                                                          // ***** debug *****
        {                                                                           // ***** debug *****
            std::cout << v.component_id << ", "                                     // ***** debug *****
                << v.component_name << ", "                                         // ***** debug *****
                << v.quantity << std::endl;                                         // ***** debug *****
        }                                                                           // ***** debug *****
        std::cout << std::endl;                                                     // ***** debug *****
    }                                                                               // ***** debug *****
*/
	//(2) riempio components_infos
	std::ifstream ist_info {"components_info.dat"};
    if (!ist_info)
        throw Invalid();

    // store component_id, component_name, delivery_time (file components_info.dat)
    int component_id;
    std::string component_name;
    double delivery_time;

	while (ist_info >> component_id >> component_name >> delivery_time)
        components_infos.push_back(component_info{component_id, component_name, delivery_time});   // store

	ist_info.close();    // chiudo ist_info
/*
	// print component_id, component_name, delivery_time
	std::cout << "Dentro components_infos:\n";                                      // ***** debug *****
    for (auto c : components_infos)                                                 // ***** debug *****
    {                                                                               // ***** debug *****
        std::cout << c.component_id << ", "                                         // ***** debug *****
            << c.component_name << ", "                                             // ***** debug *****
            << c.delivery_time << std::endl;                                        // ***** debug *****
    }                                                                               // ***** debug *****        
    std::cout << std::endl;                                                         // ***** debug *****
*/
}

void System::order_components()
{
	if (month == 0)     // lo faccio solo al primo giro
		fill_data();    // riempio model_types che mi serve per leggere i file modelX che contengono le componenti da produrre
		
    for (auto o : orders)   // per ogni ordine
    {
        if (o.time_stamp == month)  // se il timestamp è giusto, (aggiungo così gli ordini di mese in mese)
        {
            std::vector<elettrodomestico> vec;
            std::vector<componentistica> tmp;
            for (int i=0; i < model_types.size(); ++i)  // per ogni modello
            {
                if (o.model_id == model_types[i].model_id)  // se l'id del modello che cerco corrisponde
                {
                    std::vector<component> vettore = models_infos[i];   // mi seleziono quel vettore dentro models_infos (quel vettore che ha le info sul modello che cerco)
                    for (auto elem : vettore)                           // e per ogni suo elemento, inserisco nel vettore month_orders il model_id, la quantità e un ulteriore vettore tmp con le info
                    {
                        int target = elem.component_id;
                        int mesi_di_attesa;
                        for (auto x : components_infos)
                        {
                            if (x.component_id == target)
                                mesi_di_attesa = x.delivery_time;
                        }
                        tmp.push_back(componentistica{o.quantity_ordered*elem.quantity, target, mesi_di_attesa});
                    }
                    vec.push_back(elettrodomestico{o.model_id, o.quantity_ordered, tmp});
                }
            }
            month_orders.push_back(vec);
        }
    }
/*
    // print month_orders
    std::cout << "Dentro month_orders:\n";                                          // ***** debug *****
    for (auto vect : month_orders)                                                  // ***** debug *****
    {                                                                               // ***** debug *****
        for (auto x : vect)                                                         // ***** debug *****
        {                                                                           // ***** debug *****
            std::cout << x.model_id << ", "                                         // ***** debug *****
                << x.quantita_modello << "\n"                                       // ***** debug *****
                << "Le componenti per questo modello sono:\n";                      // ***** debug *****
            for (auto elem : x.componenti)                                          // ***** debug *****
            {                                                                       // ***** debug *****
            std::cout << elem.quantita_da_produrre << ", "                          // ***** debug *****
                << elem.componente << ", "                                          // ***** debug *****
                << elem.mesi_attesa << std::endl;                                   // ***** debug *****
            }                                                                       // ***** debug *****
        }                                                                           // ***** debug *****
        std::cout << std::endl;                                                     // ***** debug *****
    }                                                                               // ***** debug *****
*/
}

