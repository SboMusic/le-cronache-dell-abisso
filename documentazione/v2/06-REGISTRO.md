# Registro delle modifiche e delle decisioni

## Regola di aggiornamento

Una voce per intervento con ID, data, commit di partenza, file/area, motivo, effetto sul giocatore, prove, compatibilità e stato. I commit contengono il dettaglio completo dei file; questo registro ne spiega il significato. Non registrare una feature pianificata come implementata. Aggiungere il commit conclusivo alla voce nell'intervento successivo oppure consultare la storia Git del file, evitando hash auto-referenziali.

## V2-000 — 20 ottobre 2026 — Preproduzione e isolamento

- Base: b68c51dfa3c5568195b798791c74d8f4358ed55b; release originale v1.0.0 sul commit 49779c1.
- Stato: documentazione e strumenti di isolamento completati; nuove meccaniche non implementate.
- Aggiunti sette documenti in documentazione/v2: indice, inventario, design, audit tecnico, piano/versioni, raccordo narrativo e questo registro.
- Creati ramo release/1.x e cartella/ramo develop/2.0; backup Git completo verificato prima dei cambiamenti.
- Avvio anteprima con directory dati distinta; vecchio avvio ufficiale della sola copia 2.0 rinviato all'anteprima. Blocco creazione pacchetti sul ramo di progettazione.
- Aggiornati README, indice italiano, regole di inclusione dei documenti e manifesto dei sorgenti nella sola copia 2.0.
- Conservati main, tag v1.0.0, installer pubblici, sorgenti/asset di gioco e salvataggi personali. Nessuna modifica ai file progetto/src, agli asset o ai test.
- Verifica: dieci suite modello della base riuscite (801 controlli nelle nove suite numerate più guerre finali di entrambe le fazioni con economia pagata). Verifica di link locali, manifesto, differenze dei sorgenti e sintassi script descritta nel controllo finale dell'intervento.

## Decisioni consolidate dalla conversazione

| ID | Decisione | Stato |
| --- | --- | --- |
| DEC-01 | Rimanere fantasy, anche per le navi volanti | Concordato |
| DEC-02 | Mappe molto più grandi, più foreste e vera guerra navale | Concordato; dimensioni esatte proposte |
| DEC-03 | Strutture fino al IV, bonus economici e steli | Concordato; matrice e percentuali aperte |
| DEC-04 | Riparatori automatici, difesa, pattuglie e allarmi | Concordato; priorità dettagliate proposte |
| DEC-05 | Cristalli con specialisti e artwork dedicati | Concordato; nomi e deposito proposti |
| DEC-06 | Nuove unità umane e oscure elencate nel design | Traccia concordata; sostituzioni aperte |
| DEC-07 | Barriere circa 10 s e specialisti anti-barriera | Concordato; contromisure dettagliate proposte |
| DEC-08 | Rune temporanee assegnabili | Concordato; tipi e costi proposti |
| DEC-09 | Trasformazione umano↔oscuro, rischio e guarigione anche dopo corruzione | Concordato; irreversibilità scartata |
| DEC-10 | Purificazione attiva e lunga ricarica individuale | Concordato; contenimento/scorta e numeri proposti |
| DEC-11 | Paralisi circa 10 s e strutture protettive di entrambe le stirpi | Concordato; stacking e immunità proposti |
| DEC-12 | Storia aggiornata e sistemi introdotti progressivamente | Concordato; nuovo arco e campagna autonoma proposti |
| DEC-13 | Preparare documenti e analisi prima delle nuove meccaniche | Concordato, rispettato in V2-000 |

## Modello per i prossimi interventi

ID e data; milestone; commit iniziale; requisiti Dxx/decisioni; modifica effettiva; differenza visibile; sorgenti e asset coinvolti; dati/salvataggi; test eseguiti ed esito; prova giocata; problemi residui; stato (proposto/in corso/completato). Usare CHANGELOG per riepiloghi di release e questo file per lo sviluppo della 2.0.
