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

## V2-001 — 21 ottobre 2026 — Fondamenta delle mappe e salvataggi

- Base: 5d75e92, sviluppo autorizzato dall'utente dopo la progettazione.
- Milestone: prima parte di M1; M1 non ancora completata.
- Requisiti: D02, D03 (fluidità futura), D04 (confini raccolta), isolamento e persistenza del piano.
- Terreno, navigazione ed esplorazione passano da array fissi a contenitori dimensionati sulla mappa. Ridimensionamento validato prima dell'allocazione e conservazione opzionale per coordinate.
- La ricerca di percorso usa memoria proporzionale alla mappa; algoritmo BFS ancora presente, ottimizzazioni e benchmark realistici rinviati al prossimo incremento.
- Formato mondo 4, lettura dei formati 1–3 mantenuta con i limiti delle rispettive versioni. Ripristino in un mondo temporaneo e rifiuto dei dati incompleti restano attivi. Camera e scoperta profilo accettano coordinate oltre i vecchi bordi.
- Corretta la piantumazione: limiti basati sulla dimensione effettiva anche in verticale, non più x=2535 fisso.
- Versione sviluppo in Version.h, titolo e risorsa Windows coerenti; VERSION=2.0.0-dev.1. L'eseguibile usa il profilo ChroniclesOfTheAbyss-2-preview anche senza wrapper.
- Disposizione, obiettivi e dimensioni delle missioni esistenti restano invariati. Il supporto tecnico a mappe grandi non equivale a nuove mappe già progettate.
- Aggiunta suite map_foundation_tests e regressioni native di salvataggio camera/profilo su 192×120. Esiti nel rapporto VERIFICA-M1-001.json aggiornato al termine della verifica.
- Nessuna modifica su main, release/1.x, tag o pacchetti 1.0; nessuna migrazione automatica dei salvataggi personali. La 1.0 non può aprire i nuovi file formato 4.
- Limiti residui: tetto entità/file, costo di scansioni e percorsi, identità/controllo per rinnegati, nuova economia e mappe di missione ancora da implementare. Nessun installer 2.0 pubblicato.

Esito V2-001: compilazione riuscita, undici suite modello (831 controlli numerati più guerre finali), dieci modalità native riuscite; 133 controlli campagna inclusi due nuovi per camera e profilo. Il primo collegamento ha restituito LNK1101 nell’ambiente di compilazione; ripetuto in sequenza è riuscito. Nessun installer creato. CI estesa al ramo develop/2.0.

## V2-002 — 21 ottobre 2026 — Regioni progressive e libertà economica

- Richiesta: mappe realmente più grandi a ogni missione, villaggio redistribuito, obiettivi che non blocchino i posti liberi, operai non incastrati tra alberi e comando.
- Dimensioni delle nuove partite: missione 1 4480×2800, poi +320×200 per capitolo, fino al finale 7680×4800. Crescita per entrambe le stirpi. Risorse, strade regionali, foreste distribuite e coste/isole nelle missioni navali occupano l'estensione; gli obiettivi militari avanzati restano nei loro nuclei in attesa della riprogettazione completa.
- Villaggio della prima missione spostato nella parte orientale della regione e collegato con un sentiero; indicatore e clic contestuale seguono la posizione reale, anche nei salvataggi precedenti.
- Eliminata la condizione separata «deve esistere una fattoria» in reclutamento e uscita delle unità. Rimangono capacità iniziale, costo, requisiti tecnologici e posti riservati dalle code. Fattorie necessarie soltanto per aumentare la capacità quando esaurita; stessa regola per l'IA.
- Radure attorno a costruzioni e lavoratori, percorsi principali senza alberi invadenti; conservati gli ID degli oggetti. Movimento: se la separazione fra unità spingerebbe un operaio contro un ostacolo, prova il passo diretto valido invece di restare fermo.
- Nuove verifiche di regioni per entrambe le stirpi, distanza/raggiungibilità villaggio e due minuti di raccolta/consegna senza fattorie; aggiornate le prove che richiedevano la vecchia regola.
- Versione 2.0.0-dev.2. Per vedere la nuova disposizione avviare una nuova campagna o ricominciare la missione: caricare un salvataggio conserva deliberatamente il suo mondo, senza cancellare costruzioni o risorse del giocatore.
- Limiti: non è ancora il rifacimento di tutti i layout nemici; grafica sostitutiva e meccaniche 2.0 successive non incluse. Verifiche definitive nel rapporto VERIFICA-REGIONI-002.json.

Approfondimento V2-002: aggiunto controllo di passaggio libero prima di anticipare una svolta; operai e cisterne mantengono un avanzamento minimo quando la separazione dalle truppe ferme li arresterebbe. Verificate uscita da casella bloccata, svolta attorno a ostacolo e passaggio oltre un soldato alleato fermo. La prova di guerra ha rilevato un cantiere lontano bloccato da un gruppo alleato, usato per diagnosticare il problema. Sono state provate strategie di espansione per diagnosticare gli accessi; la prova finale conserva la strategia militare di riferimento e adatta soltanto la distribuzione e il ribilanciamento dei raccoglitori, senza dipendere dagli ID dei nuovi alberi. Nessuna concessione di risorse o riduzione degli avversari.

Esito definitivo V2-002: compilazione riuscita; dodici suite modello superate (911 controlli numerati più guerre finali vinte da entrambe le stirpi); dieci modalità native superate, inclusi 133 controlli della campagna. Aggiunte riserve intermedie e boschi produttivi accessibili senza ostruire il comando; tutte le risorse restano finite. Screenshot reale e rapporto VERIFICA-REGIONI-002.json inclusi. Nessun installer finale pubblicato.

## V2-003 — 21 ottobre 2026 — Ricostruzione dei livelli e attività degli operai

- Base: c65af69. Richiesta: superare l'estensione casuale, rifare tutte le mappe, aggiungere lavoratori rivali nel livello 1 e migliorare le azioni dei lavoratori.
- Undici layout completi in RtsRegions.inl: nuove posizioni e terreno, boschi continui, lotti più distanziati, risorse con ruoli specifici, coste e percorsi di sbarco. Aggiornati riferimenti degli obiettivi, difese e posizioni dell'IA.
- Campo opzionale nel livello 1: comando/fattoria e tre lavoratori rivali, raccolta vera, risposta difensiva e nessuna produzione/onda militare.
- A* e approcci riservati alle risorse/depositi; ricerca del bordo accessibile del bosco. Conservate le correzioni di uscita e precedenza degli operai della dev.2.
- Visualizzazione interpolata, passo legato alla distanza, animazioni e utensili distinti, trasporto visibile e scala dei lavoratori rivista. Riutilizzo degli artwork, non nuova illustrazione bitmap completa.
- Save schema 5 per i layout nuovi; lettura 1–4 preservata. Nessuna modifica ai profili della 1.0.
- Aggiunti WorkerAnimation.h, authored_maps_tests.cpp e documentazione dedicata. I vecchi casi a coordinate fisse sono dichiarati legacy; nuove prove specifiche sui layout effettivi, senza presentare le fixture di vittoria come campagne interamente giocate.
- Le verifiche hanno rilevato una foresta che chiudeva il faro: aggiunti percorsi espliciti dall'approdo all'obiettivo. Esiti finali e immagini nel rapporto VERIFICA-RICOSTRUZIONE-003.json al completamento.
- Milestone: M2 avanzata e prima revisione M5; nuove unità della 2.0, rune e altre meccaniche future non implementate qui. Bilanciamento completo delle mappe avanzate ancora da provare giocando.

Esito definitivo V2-003: compilazione dev.3 riuscita; tredici suite modello superate (2166 controlli numerati, di cui 1255 sui nuovi layout, più guerre storiche per entrambe le stirpi); dieci modalità native superate, inclusi 177 controlli campagna. Corretta la traduzione integrale del nuovo briefing inglese e il disegno delle strade, che ora segue la griglia dei nuovi layout. Controllate visivamente base iniziale, costa, arcipelago e finale. Rapporto VERIFICA-RICOSTRUZIONE-003.json; nessun installer finale, bilanciamento avanzato ancora da giocare.

## V2-004 — 21 ottobre 2026 — Cantieri e animazioni

- Base 91421ba. Richiesta: migliorare operai/raccoglitori e soldati, con strutture trasparenti che si riempiono in modo fluido.
- ActorAnimation.h: riempimento verticale sfumato, sagoma tenue, posa del colpo e deformazione articolata leggera delle figure. Gameplay conserva l’avanzamento visivo e segue la progressione reale del cantiere; il completamento rende l’artwork pienamente opaco.
- WorkerAnimation: sollevamento, impatto e recupero distinti, ritmi specifici; eliminata la sovrapposizione fra utensile dipinto e utensile animato. Cammino e orientamento legati al movimento reale.
- Soldati: passo dalla distanza, movimento di gambe/busto, affondo verso il bersaglio e recupero dalla ricarica effettiva. Scia solo all’impatto, lampo breve per gli attacchi a distanza. Stesse regole per gli oscuri. Nessuna modifica alla simulazione del danno o ai salvataggi.
- GameplayAnimationChecks.inl aggiunge prove sul framebuffer delle due fazioni e una sequenza dimostrativa opzionale. La prima verifica ha rilevato accumulo di alpha sovrapponendo sagoma e parte piena: corretto usando fasce non sovrapposte.
- Versione 2.0.0-dev.4, copia 1.0 e riferimenti stabili conservati. Artwork riutilizzati, nessun nuovo atlante completo né installer finale.

Esito V2-004: compilazione riuscita, tredici suite modello superate (2166 controlli numerati più guerre storiche), dieci modalità native superate. Grafica: 118 controlli funzionali più 25 controlli opzionali per la cattura, totale 143 nella prova registrata. Sequenza e fotogrammi verificati visivamente; rapporto VERIFICA-ANIMAZIONI-004.json.

## V2-005 — 21 ottobre 2026 — Leggibilità del cantiere e utensili

- Rimossa la barra verticale ciano/rossa che indicava la fazione degli edifici: l’anello a terra e la selezione sono sufficienti.
- Rimosse le grandi aste schematiche dell’impalcatura. Piccola catasta di tavole con facce, venature e ombra, presente solo durante il cantiere; conservata la costruzione trasparente progressiva.
- Utensili più piccoli: manico delineato e impugnatura, martello metallico sfaccettato con luce/ombra, piccone e ascia con profilo distinto. Aggiunta la mano sull’impugnatura; nessun nuovo artwork esterno.
- Versione 2.0.0-dev.5. Compilazione e controlli nativi di grafica (143 con cattura), comandi (10) e avvio autonomo (25) superati. Controllato il fotogramma dimostrativo aggiornato. Nessuna modifica alle regole o ai salvataggi; non è necessario ricominciare la partita. La 1.0 resta invariata.

## V2-006 — 29 ottobre 2026 — Passo e carichi dei lavoratori

- Ridotto il rimbalzo del busto, appoggio del passo più regolare; griglia articolata più fine con flessione del ginocchio e movimento contrapposto delle spalle. Riutilizzo dei fotogrammi esistenti, nessun nuovo atlante bitmap completo.
- Sostituiti i rettangoli della legna con tre tronchetti cilindrici: corteccia sfumata, venature, estremità con anelli, corde e ombra. Sacco d’oro con sagoma asimmetrica, facce illuminate, pieghe, cuciture e apertura legata con pepite visibili.
- Carichi aderenti al fianco e oscillazione lieve legata al passo. Stessa rappresentazione per operai e imp.
- Build 2.0.0-dev.6 compilata; 145 controlli grafici nativi superati, inclusi 25 della cattura opzionale e nuovi controlli sui carichi distinti per le due fazioni. Fotogramma 12 controllato visivamente. Non rieseguite le suite economiche: nessuna modifica alle regole della simulazione. Salvataggi invariati; non serve ricominciare la campagna.
- Anteprima dimostrativa controllata in carichi-dev6.gif; non rappresenta una partita reale. La versione 1.0 resta invariata.

## V2-007 — 29 ottobre 2026 — Utensili della stirpe oscura

- Varianti dedicate per gli imp, selezionate dalla fazione dell’entità e non da quella del giocatore: martello cornuto in metallo annerito con nucleo rosso, piccone ricurvo d’ossidiana con bordo violetto e legatura rossa. Manico scuro e mano coerente con il colore dell’imp.
- Utensili umani e tempi delle animazioni conservati; nessuna modifica a regole o salvataggi. Disegni procedurali, nessun nuovo atlante bitmap.
- Build 2.0.0-dev.7 compilata; 147 controlli grafici nativi superati (25 relativi alla cattura opzionale), compresi confronti dei soli utensili fra le due fazioni. Fotogramma dimostrativo 12 controllato visivamente. Nessun installer finale o modifica alla 1.0.

## V2-008 - 29 e 30 ottobre 2026 - Integrazione funzionale alpha 1

- Richiesta: procedere con gli altri sistemi concordati, poi rivedere i problemi e consegnare una guida completa. Base 3d1300f; ramo develop/2.0, copia stabile preservata.
- RtsCatalog e RtsV2: quarta risorsa finita, minatore specializzato e Reliquiario senza blocco iniziale sui cristalli; sei nuove strutture e undici ruoli di unità condivisi o esclusivi. Catalogo complessivo 58 tipi, 24 strutture, 25 unità reclutabili per fazione più spirito temporaneo. Atlas originale a 36 celle creato con imagegen; prompt e provenienza salvati.
- Livelli I-IV: sviluppo del Comando, miglioramento individuale degli edifici, produzione e popolazione delle fattorie, bonus limitati di salute e addestramento; steli locali non cumulative. Le risorse finite vengono prelevate per il carico effettivo, senza rendita passiva.
- Automazione: riparatore locale con spesa e riserva, presidio, pattuglia a due punti, riserva difensiva richiamata dalle torri, ritorno e limite d’inseguimento. Ordini manuali prioritari, cantiere ancora esclusivo.
- Rune prodotte e pagate, stock limitato e consumo per destinatario valido. Barriera di 10 s con ricarica condivisa, spezzabile con rituale. Paralisi annunciata di 10 s con resistenza per comandanti, immunità successiva e obelisco protettivo.
- Trasfigurazione: 25 s di potere, 10 s di instabilità, ostilità a entrambe le parti e contenimento curabile con due scorte. Trattamento 18 s e recupero individuale 240 s persistente. Cure e ricostruzione non richiedono cristalli, per evitare un blocco irreversibile all’esaurimento delle cave. Corrotti riconoscibili e senza visuale regalata al proprietario; proiettili dei rinnegati gestiti anche contro gli alleati originari.
- Nuovi reparti e trasporti aerei utilizzano il sistema reale di produzione, popolazione, movimento, rilevamento e sbarco. IA usa gli stessi costi per cristalli, unità, strutture, rune e abilità; trasporti navali con passeggeri reali tentano sbarchi nelle missioni pertinenti.
- Comandi contestuali, cataloghi, risorse, aree, stati, briefing e dizionario IT/EN aggiornati. Nomi degli edifici visibili alla selezione/avvicinamento del mouse per ridurre sovrapposizioni. Spazio torna al comando reale; U supporta anche l’arca volante.
- Formato mondo 6 con stato tattico e validazione; lettura 1-5 conservata. Import del formato 5 integra le nuove cave senza ricostruire il villaggio. Le fixture storiche rimangono esplicite; nuove suite modello e native per i sistemi effettivi.
- Interruzione del controllo automatico per limite d’uso il 29 ottobre: il comando respinto non fu eseguito. Ripreso il lavoro il 30 ottobre dopo l’istruzione del giocatore.
- Guida completa in Markdown e PDF, inventario riproducibile con catalog_export.cpp. Stato alpha, non dichiarazione di bilanciamento definitivo o completamento artistico. Nessuna nuova campagna oscura autonoma, audio, multiplayer o editor; nessun installer stabile o modifica ai tag 1.0. Esiti finali in VERIFICA-ALPHA1.json.

Esito V2-008: compilazione 2.0.0-alpha.1 riuscita; 14 suite modello superate (2331 controlli numerati più guerre storiche pagate), 10 modalità native superate, inclusi 197 controlli campagna. Nuovo caso di importazione v5 verificato dopo la suite generale; il primo file di prova aveva un errore nella sostituzione dell’intestazione ed è stato corretto. Nessun errore di importazione effettivo rilevato. PDF completo di 10 pagine renderizzato e controllato; guida copiata accanto all’eseguibile. Rapporto e hash in VERIFICA-ALPHA1.json. Resta il bilanciamento giocato delle mappe ricostruite, distinto dal superamento delle fixture.
