// ***** SystemProcessing.cpp *****

#include <fstream>
#include "System.h"

using namespace std;

bool finished(int m, const vector<order>& o)    // controlla se il main deve terminare
{
    return m > o[o.size() - 1].time_stamp + 12; // il +12 è per indicare che lasciamo un anno a partire dal time_stamp dell'ULTIMO ORDINE per completarne la consegna
}

void fill_orders(vector<order>& o, istream& i)  // riempre il vector da orders.dat
{
	int time_stamp;
	int model_id;
	int quantity_ordered;
	while (i >> time_stamp >> model_id >> quantity_ordered)
		o.push_back(order{time_stamp, model_id, quantity_ordered});
}

int main()
{
	int month = 0;                  // tengo traccia del mese corrente
	System syst;                    // sistema di gestione degli ordini
    
	ifstream ist {"orders.dat"};    // input orders.dat
	if (!ist)
		throw Invalid();

	// riempio orders dal file orders.dat
	fill_orders(syst.orders, ist);
	ist.close();                    // chiudo ist
/*
	// print orders                                                                         // ***** debug *****
	std::cout << "Dentro orders:\n";                                                        // ***** debug *****
    for (auto o : syst.orders)                                                              // ***** debug *****
    {                                                                                       // ***** debug *****
        std::cout << o.time_stamp << ", "                                                   // ***** debug *****
            << o.model_id << ", "                                                           // ***** debug *****
            << o.quantity_ordered << std::endl;                                             // ***** debug *****
    }                                                                                       // ***** debug *****        
    std::cout << std::endl;                                                                 // ***** debug *****
*/
	// loop principale del programma!
	for (month = 0; !finished(month, syst.orders); month++)
	{
		syst.check_situation(); // invocata una volta per ogni mese -> incrementa il mese del System
	}
}

