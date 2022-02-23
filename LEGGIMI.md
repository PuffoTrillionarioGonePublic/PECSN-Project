# Progetto 8 – Torre di Controllo

Una torre di controllo gestisce il traffico aereo di un aeroporto. L'aeroporto dispone di una pista di atterraggio/decollo e di un parcheggio per gli aerei. La pista può essere occupata da un aereo alla volta, mentre l'area di parcheggio può contenerne uno o più contemporaneamente. Il parcheggio ha una capienza infinita.
Ogni volta che un aereo raggiunge l'area dell'aeroporto, segue i seguenti passaggi:
1. l'aereo si mette in coda per l'atterraggio fino a quando non riceve l'ok dalla torre di controllo (supponendo che gli aerei abbiano sempre abbastanza carburante per rimanere in aria per un tempo arbitrario);
2. il velivolo effettua l'operazione di atterraggio, che dura un tempo tl;
3. l'aereo rimane nell'area di parcheggio per un tempo tp;
4. l'aereo si mette in coda per il decollo fino a quando non riceve l'ok dalla torre di controllo;
5. l'aereo decolla in un tempo to ed esce dal sistema.

Quando la pista non è occupata, la torre di controllo serve un aeroplano secondo la seguente politica:
1. servire un aereo in coda per l'atterraggio;
2. se la coda di atterraggio è vuota, servire un aereo in coda al decollo.

Si consideri il seguente carico di lavoro: i tempi di interarrivo dell'aeroplano sono RV IID (descritte più avanti), i valori tl, to e tp sono RV IID (descritte più avanti).

Modella il sistema sopra descritto e studia il tempo di attesa in coda sia in atterraggio che in decollo con un carico di lavoro variabile. Studiare come varia il numero di aeroplani nell'area di parcheggio in relazione a tl, tp. Più in dettaglio, devono essere valutati almeno i seguenti scenari:
- Tempi inter-arrivo costanti, tempi di servizio costanti.
- Distribuzione esponenziale di tutti i suddetti RV, con le stesse modalità del caso precedente.

In tutti i casi spetta al team calibrare gli scenari in modo da ottenere risultati significativi.

Risultati attesi del Progetto :

a) Documentazione (secondo gli standard fissati durante le lezioni)

b) Codice del simulatore

c) Presentazione (massimo 10 slide)
