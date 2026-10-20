# Analisi tecnica e strategia di modifica

Analisi statica del sorgente b68c51d e verifica dinamica della base effettuata il 20 ottobre 2026. Nessuna meccanica 2.0 implementata. Le misure di prestazione su grandi mappe non sono state eseguite: i rischi di scala descritti sotto sono deduzioni dalla struttura del codice, non risultati di benchmark.

## 1. Base e punti da conservare

C++17, SFML 2.5.1, piccolo strato NGin modificato, progetto Visual Studio Win32. `FrontiereMain.cpp` gestisce menu e campagne; `Gameplay.cpp` e inclusioni `.inl` gestiscono interazione e disegno. `AlphaSimulation.h` dichiara World ed Entity e include l'implementazione di simulazione. Non è un ECS: World contiene vettori di entità, proiettili, effetti e zone e un'entità concentra molti campi facoltativi.

Punti validi: ID delle entità già presenti, cataloghi centralizzati, simulazione a passo fisso, separazione di base tra simulazione e disegno, produzione pagata e requisiti comuni, salvataggi espliciti e transazionali nel ripristino, prove di economia e guerre complete. La produzione finita è accodata per ID prima di aggiungere entità: conservare questa precauzione. Non serve riscrivere tutto o cambiare motore per principio; prima refactoring mirati e misure.

## 2. Evidenze e impatto

| Area | Evidenza nel sorgente della base | Conseguenza per la 2.0 |
| --- | --- | --- |
| Dimensioni | `AlphaSimulation.h`: maxCells=5760; array terreno, nav e visibilità a capacità fissa. `RtsCampaignLater.inl`, expandMap: 96×60. | 192×120 richiederebbe 23040 celle; nessun semplice cambio di cols/rows è sicuro. |
| Ricerca percorsi | `RtsWorld.inl:65–75`: BFS a quattro vicini con array parent e coda per ogni ricerca. | Valutare A*, smoothing dei segmenti, budget per frame e riuso delle richieste; percorsi validi prima della sola estetica. |
| Movimento | `RtsWorld.inl:248–252`: separazione attraverso tutte le entità mobili e rimozione del primo nodo del vettore. | Rischio di costo quadratico con eserciti grandi; indice spaziale e cursore del percorso sono candidati. |
| Ricerca entità | `RtsWorld.inl:3–4`: get(id) scorre il vettore; molti cicli annidati per bersagli, risorse e vicini. | Mappa ID→indice e query locali prima di aggiungere migliaia di alberi/aure. |
| Limite residuo concreto | `RtsWorld.inl:150`: plant rifiuta x>2535, non usa width(). | La piantumazione è già limitata rispetto alla mappa ampliata; correggere nella milestone mappe con prova ai margini. |
| Risorse | `RtsCatalog.h` Cost ha tre campi; World separa gold/wood/oil dai corrispondenti enemy*. pay, cargo e depositi hanno casi specifici. | Quarta risorsa coinvolge prezzi, raccolta, IA, HUD, rimborsi e salvataggi insieme. |
| Livelli | `RtsWorld.inl:154–174`: Town2/Town3 e Barracks2/3 dedicati; townLevel per squadra, livello locale caserma. | Servono regole esplicite di livello locale e tecnologia globale prima di aggiungere IV. |
| Ordini | `AlphaSimulation.h`: Idle, Move, Gather, Return, Attack, Repair, Board, Build. | Ruolo automatico e ordine corrente vanno distinti, altrimenti una riparazione cancella la pattuglia o un allarme interrompe il cantiere. |
| Effetti | Entity ha haste, rage, shield, reveal e un solo spellCooldown; Spell/Research sono enumerazioni indicizzate. | Non aggiungere una serie incontrollata di timer: definire effetti, sorgenti, categorie, priorità e cooldown per abilità. |
| Fazioni | faction[2], team -1/0/1, filtro speciale Mage/Warlock; molte regole assumono nemico=1. | Il corrotto ostile a tutti non è rappresentabile come semplice cambio di faction. Servono proprietà, controllo e relazioni separati. |
| Dati sovraccarichi | `RtsWarCampaign.inl` e renderer usano lifetime=-777 per il capo; work/level hanno impieghi anche di obiettivo. | Non riusare questi campi per corruzione o livello IV: introdurre stati e marcatori espliciti. |
| Persistenza | `RtsSave.inl`: FRONTIERE_SAVE 3, lettura versioni 1–3; bounds 96×60, livello<=3, entità<=4096, team<=1. | Nuove mappe, livelli, entità e controllo richiedono uno schema nuovo, validatori aggiornati e conversione. |
| Profili | `CampaignFiles.h`: directory storica Frontiere, override FRONTIERE_DATA_DIR, inspect limite 4 MiB, involucro campagna versione 1. | Anteprima isolata; limite file da ricalcolare con misure e validazioni, non rimuovere ogni protezione. |
| Disegno | `GameplayTerrain.inl`: terreno visibile parzialmente filtrato dalla camera; renderer ricostruisce minimappa/fog e ordina entità. | Conservare culling esistente, introdurre cache delle parti statiche e query visibili dove utile; evitare rifare tutto ogni frame. |
| Animazioni | `Gameplay.cpp` Visual tiene direzione/fase; sprite iniziali a quattro direzioni e piccoli cicli, rotazioni/bobbing; GameplayActivity disegna attrezzi. | Sistema di clip per stato/direzione e transizioni; interpolazione grafica proposta senza alterare i tempi della simulazione. |
| Campagna/IA | RtsCampaign*.inl e RtsWarEconomy.inl combinano coordinate, missioni, obiettivi ed economia in codice; faction conversion adatta la stessa campagna. | Non scalare tutte le coordinate alla cieca; definire mappe/scenari e obiettivi indipendenti, poi due percorsi narrativi. |
| Localizzazione | Localization.h e TSV inglese con testo italiano come riferimento. | Ogni nuova azione, requisito e rifiuto deve avere traduzione; considerare chiavi stabili per i nuovi contenuti. |
| Distribuzione | VERSION, risorsa Windows, frontend, script e guide hanno versioni/nome in più punti. | Centralizzare versione e canale prima di creare pacchetti 2.0; non rinominare globalmente FRONTIERE nei formati legacy. |

Le righe citate si riferiscono alla base, non devono essere trattate come posizioni immutabili dopo il refactoring. Per riprodurre la lettura: consultare il ramo release/1.x o il commit b68c51d.

## 3. Modello di modifica consigliato

### Mondo e navigazione

Estrarre una mappa con dimensioni e buffer coerenti, indici e controlli di confine centralizzati. Rendere esplicito cellSize dove ora ricorre 40; distinguere coste reali da costante coast=1200 del prototipo. Conservare griglie distinte terra/mare e regole di atterraggio/sbarco. Iniziare da una mappa di prova grande e misurare ricerca percorsi, visibilità, disegno e dimensione salvataggio separatamente.

Indice spaziale per alberi, edifici e unità, aggiornamenti nav locali a costruzione/abbattimento; nessuna ricostruzione totale a ogni colpo. A* e smoothing sono candidati, non decisioni dimostrate migliori prima dei benchmark. Per grandi gruppi valutare percorsi condivisi e destinazioni distribuite dopo una prima misura. Alberi morti/corpi non possono essere cancellati senza conservare ciò che serve a riassegnazione raccolta, evocazioni e riferimenti di missione.

### Identità, proprietà e stati

Mantenere ID stabili e risolvere gli accessi a World in modo controllato. Non conservare puntatori a elementi di vector dopo operazioni add/erase; controllare i call site durante il refactoring senza dichiarare ora un crash non riprodotto. Separare tipo di unità, stirpe grafica/trasformata, proprietario originario, controllore attuale e ostilità. Le unità rinnegate non vanno confuse con risorse neutrali, perché team=-1 oggi ha un altro significato.

Separare ordine manuale, ruolo, zona assegnata e coda di attività. Ordini di difesa/pattuglia devono salvare punto di ritorno e percorso. Creare una regola di priorità comune per cantiere, trattamento, contenimento, paralisi e comandi. Unità corrotte gestite da comportamento dedicato con possibilità di recupero; definire sconfitta e popolazione: proposta, riservare capacità al proprietario durante recupero per impedire eserciti oltre limite.

### Economia e progressione

Portafoglio per squadra e tipo di risorsa, prezzi e rimborsi coerenti. Catalogo di raccoglitori, giacimenti e depositi, invece di una nuova condizione Worker in ogni funzione. Stabilire se una stessa caverna viene raffinata secondo la stirpe. Tempi di estrazione/costruzione calcolati da un'unica aggregazione di modificatori con limiti e sorgenti, così Comando IV, steli e rune non si moltiplicano accidentalmente. Non modificare i costi delle unità già in coda quando cambia una ricerca.

Dati di livello per edificio e requisiti di sblocco, mantenendo separata conoscenza globale e livello locale. Distruzione di Comando/stele deve invalidare soltanto i bonus che dipendono dalla sua presenza, non tutte le tecnologie. Rendere testabili costi e disponibilità per entrambi i team attraverso lo stesso percorso di regole.

### Effetti, abilità e contromisure

Registro di effetti attivi con sorgente, durata, categoria, stacking e immunità; cooldown per abilità e per unità. Barriera, paralisi e corruzione sono sistemi distinti. Runa consumata una volta per destinatario valido; corruzione persistente non eliminabile dalla dissoluzione ordinaria. Timer e comportamento devono progredire nel passo di simulazione, non nelle animazioni. Controllare tutte le fonti di danno, compresi proiettili già in volo, aree e demolizione quando nasce una barriera.

Eventi di allarme e ordini di guarnigione per zona con visibilità corretta; IA chiama le medesime azioni pagate. Evitare un nuovo scansione-completa-per-altare-per-frame: query spaziali e intervalli deterministici sono candidati.

### Presentazione e contenuti

Manifest di asset con tipo, fazione, livello, stato e clip; caricamento controllato e messaggi chiari per file mancanti. Conservare atlanti della 1.0 fino a sostituzione verificata. Interpolazione fra stati della simulazione per fluidità, senza spostare hitbox o raccogliere più velocemente per effetto degli FPS. Mappe a settori per rendering statico, animazione dell'acqua separata, minimappa aggiornata su cambiamenti ed esplorazione. Non è necessario migrare a SFML 3 nel medesimo intervento: aggiornamento dipendenze è un lavoro separato da valutare.

### Salvataggi e migrazione

Nuovo schema versionato, ID di contenuto stabili oppure mappatura esplicita dei vecchi enum; mai inserire nuove voci in mezzo agli ID serializzati della 1.0 senza migrazione. Salvare risorsa cristalli, mappe, ordini persistenti, origini/forme, timer, rune, posti cura e obiettivi. Scrittura atomica e lettura in un mondo temporaneo da mantenere. Importazione 1.0 soltanto su copia e test dedicati; nessuna promessa di compatibilità automatica delle missioni riscritte. Rollback con salvataggi 1.0 originali, non apertura inversa di un file 2.0.

## 4. Verifica della base eseguita

Comando: strumenti/Verifica.ps1 -SoloSimulazione, nella cartella 1.x. Esito: riuscito.

| Suite | Esito |
| --- | --- |
| alpha_tests | 41 controlli |
| roster_tests | 260 |
| mechanics_tests | 30 |
| forest_tests | 59 |
| progression_tests | 34 |
| campaign_tests | 125 |
| war_tests | 118 |
| cheat_tests | 64 |
| campaign_slots_tests | 70 |
| final_battle_tests | guerre finali umana e oscura completate con economia pagata |

Totale delle nove suite con conteggio: 801 controlli, più la suite di guerre finali. I test di interfaccia e installer non sono stati ripetuti in questa attività di progettazione; la compilazione e 39 verifiche dell'interfaccia erano già riuscite nell'intervento precedente. Nessuno di questi esiti prova la fluidità della futura mappa 2.0.

## 5. Criteri tecnici prima dell'implementazione estesa

Benchmark ripetibile su hardware dichiarato con mappa, alberi e unità specificati; registrare mediana e picchi di tempo simulazione, percorsi, draw e salvataggio. Prove dei confini, percorsi irraggiungibili e foreste abbattute. Verifica della nuova persistenza prima di rune/corruzione. Piccola scena completa per una coppia di unità e una struttura prima di generare tutti gli artwork. Regressioni IT/EN e parità IA a ogni milestone. Non aggiungere multiplayer o una riscrittura completa fra gli interventi indispensabili senza una nuova decisione.
