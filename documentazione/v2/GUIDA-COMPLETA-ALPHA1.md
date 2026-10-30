# Le Cronache dell Abisso
## Guida completa alla versione 2 alpha 1

30 ottobre 2026. Documento per Adam e per chi prova il gioco. Versione di riferimento 2.0.0-alpha.1, ramo develop/2.0.

Questa guida raccoglie ciò che è stato realizzato dalla 1.0 all’attuale anteprima 2.0, spiega i comandi e le regole e distingue le funzioni disponibili dalle verifiche ancora necessarie. Il nuovo blocco funzionale comprende cristalli, specialisti, livelli IV, automazioni, nuovi eserciti e poteri tattici. È una build alfa da provare e rifinire, non una release stabile già bilanciata in ogni missione.

## Avvio e conservazione della versione 1

Nella cartella le-cronache-dell-abisso-v2 avvia Avvia Anteprima 2.0.cmd. L’eseguibile è gioco/ChroniclesOfTheAbyss.exe. La cartella gioco contiene le librerie e gli artwork necessari; per un trasferimento bisogna copiarla interamente, non soltanto l’eseguibile. Non servono un motore a pagamento o un editor per giocare. La compilazione dei sorgenti richiede gli strumenti C++ indicati nella documentazione per sviluppatori.

I dati della 2.0 sono separati in %LOCALAPPDATA%/ChroniclesOfTheAbyss-2-preview. La copia stabile, i suoi salvataggi e la release GitHub 1.0 restano separati. Non è stato pubblicato un installer finale 2.0. Gli script di confezionamento stabile rimangono protetti per evitare sostituzioni involontarie.

Il menu offre nuova campagna, ripresa, opzioni, prologo ed esci. All’inizio scegli umani oppure stirpe oscura. Ogni campagna conserva progressi e salvataggi propri. La lingua italiana o inglese e la visualizzazione a schermo intero o in finestra si scelgono dalle opzioni. Il prologo mantiene il percorso introduttivo storico; la campagna ricostruita è il percorso consigliato per le novità.

## Comandi e interfaccia

| Azione | Comando |
| --- | --- |
| Selezionare e raggruppare | Clic sinistro; trascinamento per un gruppo; Maiusc per aggiungere |
| Muoversi o interagire | Clic destro sul terreno o sul bersaglio |
| Raccogliere | Operaio su albero/miniera; minatore specializzato su caverna; cisterna su piattaforma |
| Muovere la visuale | Tasto centrale premuto e trascinamento; frecce o WASD |
| Spostarsi rapidamente | Clic o trascinamento sulla minimappa; Spazio torna al comando |
| Ingrandire | Rotella del mouse |
| Cataloghi | F3 unità, F4 edifici, F5 ricerche, F6 magie |
| Pausa e menu | P pausa; Esc chiude il pannello o apre il menu secondo il contesto |
| Produzione rapida | O operaio; T soldato; B costruzione della caserma con operaio selezionato |
| Esercito e trasporti | H seleziona combattenti; U sceglie lo sbarco del trasporto selezionato |
| Salvataggi | Menu di pausa; F9 salva e F10 riprende dove previsti dall’interfaccia |

La mappa non segue il puntatore da solo. Il terreno inesplorato è nero; una zona scoperta rimane chiara. La presenza dei nemici dipende dalla visuale attuale e dal rilevamento. Le azioni specifiche compaiono nelle icone della selezione. I comandi numerosi hanno più pagine: usa le frecce del pannello. Costi e prerequisiti si leggono nel catalogo e nei dettagli dell’icona. La barra delle risorse mostra oro, legno, olio, cristalli e popolazione.

Gli obiettivi stabiliscono la vittoria, senza imporre l’ordine delle azioni già sbloccate. Se ci sono posti liberi e risorse sufficienti, puoi reclutare un operaio prima di costruire una fattoria. Restano requisiti tecnologici, capacità e blocco del lavoratore impegnato in un cantiere.

## Territorio ed economia

Le undici mappe sono state ricostruite con boschi compatti, radure, percorsi, miniere e luoghi di missione ridistribuiti. Ogni capitolo cresce di 320 per 200 unità di mappa: dal primo scenario 4480 per 2800 al finale 7680 per 4800. Le prime quattro missioni non contengono acqua. Coste, isole e approdi compaiono quando si impara la navigazione. Il numero di mappe resta dieci più la battaglia finale.

Alberi, miniere, giacimenti e caverne sono finiti. Quando una risorsa si esaurisce, scompare come oggetto utilizzabile e libera il terreno occupato. Il legno non ricresce gratuitamente: la ricerca Silvicoltura consente di piantare nuovi alberi pagando oro e aspettando la crescita. Le nuove caverne partono con 2400 cristalli ciascuna. Sono collocate vicino agli insediamenti e nelle zone di espansione, senza richiedere cristalli per raggiungere la prima riserva.

| Risorsa | Raccoglitore | Deposito |
| --- | --- | --- |
| Oro | Operaio o imp lavoratore | Comando |
| Legno | Operaio o imp lavoratore | Comando o segheria |
| Olio | Nave cisterna | Cantiere navale, dopo raccolta da piattaforma completata |
| Cristalli | Minatore dell’Aurora o Scavatore dell’Abisso | Reliquiario della propria fazione |

Per iniziare coi cristalli costruisci una segheria, poi il Reliquiario e recluta lo specialista. Reliquiario e primo minatore costano soltanto oro e legno. Il minatore non può costruire o riparare; cerca una caverna disponibile quando è libero e conserva il carico se il deposito non è raggiungibile. Gli operai comuni non estraggono cristalli. Aurora e Abisso sono due rappresentazioni della stessa risorsa economica.

La capacità iniziale è 10. Una fattoria di livello I aggiunge 6 posti, poi 8, 10 e 12 ai livelli II, III e IV. Il limite normale della nuova campagna è 140. Le unità più pesanti occupano più posti; anche le produzioni in corso prenotano popolazione. Le regole valgono per gli avversari: distruggere fattorie, risorse e produttori ne riduce realmente la capacità.

## Edifici dal primo al quarto livello

Il Comando II richiede una caserma; il III una fucina; il IV richiede il III e un Reliquiario, oltre all’avanzamento della campagna. Le ricerche costano rispettivamente 180 oro e 120 legno; 300 oro e 200 legno; 450 oro, 300 legno e 100 cristalli. Tempi base 18, 22 e 30 secondi. Il Comando aumenta la propria salute massima di 400 per ogni passaggio.

Per gli altri edifici usa l’icona Livello successivo sulla struttura selezionata. Il tetto dipende dal Comando e dalla tecnologia sbloccata nella missione. Passare al livello L costa 80 volte L in oro e 55 volte L in legno; il IV richiede anche 40 cristalli. Tempo base 10 più 4 volte L secondi. Ogni passaggio aggiunge il 25 per cento della salute base dell’edificio. Palizzate, piattaforme e torri seguono anch’esse questo miglioramento; l’evoluzione di una torre di vedetta in torre armata resta un’azione separata.

Il migliore Comando completato e ancora vivo accelera raccolta e costruzione del 15 per cento per livello oltre il primo, fino al 45 per cento. Più comandi non moltiplicano questo effetto. Le conoscenze già ricercate restano quando un comando cade; l’accelerazione operativa diminuisce se non rimane un comando equivalente. Un produttore più avanzato lavora il 10 per cento più velocemente per livello oltre il primo e addestra unità con salute e danno base maggiorati dell’8 per cento per livello oltre il primo.

L’estrazione comune parte da carichi di 10, più 2 per livello conosciuto del Comando oltre il primo. Una segheria aggiunge 2 al carico di legna; una raffineria aggiunge 3 all’olio. Il carico dei minatori specializzati parte da 10 e cresce con il livello del Reliquiario che li ha prodotti. I bonus aumentano quanto si preleva dalla riserva finita: non generano una rendita passiva infinita.

Le costruzioni iniziano come sagome trasparenti e si colorano gradualmente dal basso verso il tetto, seguendo i lavori reali. Un operaio assegnato costruisce e non raccoglie o combatte contemporaneamente. Decorazioni e sigilli distinguono i livelli; non sono ancora quattro nuovi artwork completi per ciascun edificio.

## Strutture e produzione

Le sigle dei costi sono O oro, L legno, P olio e C cristalli. I numeri descrivono i costi base; lo sblocco dipende anche dalla missione e dai prerequisiti del catalogo. Ogni struttura produce soltanto le unità associate.

| Umani e oscuri | Funzione | Costo O L P C | Comando |
| --- | --- | --- | --- |
| Comando / Cittadella infernale | Deposito, operai e sviluppo della citta | 300 / 200 / 0 / 0 | 1 |
| Caserma / Caserma delle fauci | Soldati; potenzia per arcieri e veterani | 120 / 80 / 0 / 0 | 1 |
| Fattoria / Nido demoniaco | Aggiunge sei posti di popolazione | 70 / 40 / 0 / 0 | 1 |
| Segheria / Segheria infernale | Deposito legno e sblocco tiratori | 100 / 80 / 0 / 0 | 1 |
| Fucina / Forgia delle braci | Ricerca armi e armature | 140 / 90 / 0 / 0 | 1 |
| Scuderia / Serraglio infernale | Addestra la cavalleria | 170 / 100 / 0 / 0 | 2 |
| Santuario / Altare dell abisso | Guaritori, custodi e dottrina veterana | 180 / 110 / 0 / 0 | 2 |
| Accademia arcana / Torre degli evocatori | Incantatori e ricerca magica | 220 / 140 / 0 / 0 | 3 |
| Officina / Officina dei tormenti | Assedio, guastatori e ricognizione | 180 / 120 / 0 / 0 | 2 |
| Voliera / Nido delle viverne | Addestra i roc da guerra | 220 / 160 / 0 / 0 | 3 |
| Torre di vedetta / Torre del teschio | Rilevamento; evoluzione in torre armata | 80 / 60 / 0 / 0 | 1 |
| Torre di guardia / Torre delle spine | Difesa a distanza e antiaerea | 140 / 90 / 0 / 0 | 1 |
| Torre di artiglieria / Torre del fuoco | Difesa ad area contro terra e navi | 190 / 110 / 0 / 0 | 2 |
| Cantiere navale / Cantiere dei dannati | Si costruisce sulla costa; produce navi | 160 / 120 / 0 / 0 | 1 |
| Fonderia navale / Fonderia abissale | Sblocca corazzate e migliora la flotta | 170 / 120 / 30 / 0 | 2 |
| Raffineria / Raffineria infernale | Aumenta il petrolio per consegna | 140 / 100 / 0 / 0 | 1 |
| Piattaforma petrolifera / Trivella infernale | Cisterna: costruisci su un giacimento in mare | 120 / 70 / 0 / 0 | 1 |
| Palizzata / Muro delle corna | Blocca i percorsi terrestri | 10 / 15 / 0 / 0 | 1 |
| Reliquiario dell Aurora / Reliquiario dell Abisso | Deposito cristalli e minatori specializzati | 160 / 100 / 0 / 0 | 1 |
| Stele sacra / Stele dell Abisso | Aura locale: forza e consegne; non cumulabile | 160 / 100 / 0 / 30 | 2 |
| Altare della Protezione / Altare del Velo Abissale | Barriera locale di 10 s, ricarica condivisa 90 s | 220 / 140 / 0 / 50 | 3 |
| Laboratorio delle Rune / Forgia dei Sigilli | Produce rune consumabili per le truppe | 190 / 130 / 0 / 30 | 2 |
| Santuario della Trasfigurazione / Altare della Trasfigurazione | Potenzia, contiene e cura la corruzione | 230 / 160 / 0 / 0 | 3 |
| Obelisco della Liberta / Pilastro della Volonta | Immunita locale alla paralisi | 180 / 120 / 0 / 40 | 2 |

La Stele dà un bonus locale di forza ed estrazione: 10 per cento al livello I e altri 2 punti per ogni livello successivo, fino al 16 per cento. Il raggio parte da 345 e cresce fino a 420. Steli sovrapposte non sommano lo stesso bonus; prevale il valore maggiore. L’Obelisco protegge dalla paralisi entro 300. Le strutture di supporto mostrano il proprio raggio quando vengono selezionate.

## Eserciti delle due fazioni

Il catalogo comprende 25 tipi reclutabili per gli umani e 25 per gli oscuri. A questi si aggiunge lo spirito temporaneo evocato dove previsto. Il numero dei ruoli nel catalogo non è il numero di sprite direzionali: alcuni artwork usano ancora animazioni procedurali.

Ippogrifi e cavalleria dell’Aurora danno agli umani mobilità e supporto. Gli oscuri hanno demoni minori, cavalieri su bestie dell’Abisso e draghi pesanti. Angeli, trasporti aerei, specialisti runici e minatori hanno varianti delle due fazioni. Il Custode umano continua a svolgere il ruolo del cavaliere sacro; non è stato duplicato come seconda unità identica.

| Umani | Oscuri | Produttore | Costo O L P C |
| --- | --- | --- | --- |
| Operaio | Imp lavoratore | Comando | 50 / 0 / 0 / 0 |
| Soldato | Diavolo guerriero | Caserma | 80 / 0 / 0 / 0 |
| Arciere | Arciere infernale | Caserma | 75 / 25 / 0 / 0 |
| Esploratore veterano | Predatore abissale | Caserma | 105 / 35 / 0 / 0 |
| Cavaliere | Cavaliere infernale | Scuderia | 150 / 35 / 0 / 0 |
| Custode | Custode dell abisso | Santuario | 190 / 60 / 0 / 0 |
| Balista | Balista demoniaca | Officina | 180 / 100 / 0 / 0 |
| Guastatore | Imp dinamitardo | Officina | 100 / 65 / 0 / 0 |
| Arcanista | - | Accademia arcana | 190 / 60 / 0 / 0 |
| - | Evocatore dell abisso | Accademia arcana | 190 / 60 / 0 / 0 |
| Guaritore | Sacerdote oscuro | Santuario | 120 / 50 / 0 / 0 |
| Aeronave da ricognizione | Occhio alato | Officina | 110 / 60 / 0 / 0 |
| Roc da guerra | Viverna infernale | Voliera | 250 / 100 / 0 / 0 |
| Nave cisterna | Cisterna infernale | Cantiere navale | 100 / 40 / 0 / 0 |
| Trasporto | Arca dei dannati | Cantiere navale | 130 / 70 / 20 / 0 |
| Fregata | Fregata demoniaca | Cantiere navale | 170 / 80 / 40 / 0 |
| Corazzata | Corazzata infernale | Cantiere navale | 260 / 150 / 100 / 0 |
| Sommergibile | Predatore sommerso | Cantiere navale | 190 / 80 / 70 / 0 |
| Minatore dell Aurora | Scavatore dell Abisso | Reliquiario dell Aurora | 75 / 20 / 0 / 0 |
| Domatore di ipogrifi | - | Voliera | 230 / 100 / 0 / 35 |
| Cavaliere dell Aurora | - | Scuderia | 230 / 80 / 0 / 45 |
| Angelo minore | Angelo caduto | Santuario | 200 / 90 / 0 / 45 |
| Paladino | - | Santuario | 210 / 80 / 0 / 35 |
| Arca dell Aurora | Vascello dell Abisso | Voliera | 320 / 180 / 0 / 80 |
| Spezzarune | Profanatore del Velo | Laboratorio delle Rune | 160 / 65 / 0 / 30 |
| Vincolatore runico | Carceriere dell Abisso | Laboratorio delle Rune | 180 / 80 / 0 / 40 |
| - | Drago demoniaco | Voliera | 310 / 150 / 0 / 70 |
| - | Predatore su bestia dell Abisso | Scuderia | 240 / 90 / 0 / 45 |
| - | Demone minore | Caserma | 120 / 35 / 0 / 15 |

Le scelte dell’altra fazione vengono escluse da cataloghi e produttori. La caserma resta locale: il II abilita arcieri, il III i veterani con gli ulteriori requisiti. Le ricerche di armi, armature e dottrina continuano a essere acquisti separati dagli edifici.

## Ruoli automatici e difesa

Riparatore auto trasforma un operaio comune in un addetto a una zona di raggio 480. Cerca edifici completati e danneggiati, paga 2 oro e 2 legno per ogni riparazione da 30 salute e torna disponibile. Sotto una riserva di 30 oro o legno non inizia nuovi interventi automatici. I cantieri già assegnati restano impegnativi fino al completamento.

Presidia memorizza la posizione e limita l’inseguimento a una zona di 320. Pattuglia alterna il punto di partenza e quello scelto con un clic, riprendendo il percorso dopo lo scontro. Difendi campo assegna una riserva locale: torri di vedetta, guardia e artiglieria vicine rilevano minacce visibili entro 420 e richiamano i difensori assegnati. Il limite della riserva è 650; non viene richiamato automaticamente l’intero esercito. Rilevamento e visuale restano necessari.

Un ordine manuale dal pannello o un clic destro sostituisce il ruolo automatico. L’icona Ruolo manuale lo disattiva esplicitamente. Un operaio impegnato a costruire non può interrompere il lavoro per assumere una nuova mansione. Le unità paralizzate o in trattamento non eseguono normali ordini.

## Barriere rune e paralisi

| Sistema | Preparazione e costo | Effetto e contromisura |
| --- | --- | --- |
| Barriera | Altare completato, 20 cristalli per attivazione | 10 s nel raggio 320; ricarica condivisa 90 s per fazione |
| Spezzasigilli | Spezzarune o Profanatore, bersaglio entro 360 | Rituale 2 s, ricarica 30 s; interrompibile da movimento, danni o paralisi |
| Rune | Laboratorio, 45 oro e 12 cristalli, 8 s base | Scorta massima 8 per tipo; una runa per unità, 30 s |
| Paralisi | Vincolatore o Carceriere, 60 mana | Rituale 1,5 s, raggio d’effetto 110, fino a 10 s; ricarica 45 s |
| Protezione dalla paralisi | Obelisco o Pilastro completato | Raggio 300; libera gli alleati e mantiene immunità locale |

La barriera protegge anche l’altare e gli alleati nel cerchio; consente di attaccare dall’interno. Può proteggere navi costiere nel raggio. Un secondo altare non aggira la ricarica condivisa. Il rituale di dissoluzione funziona senza dover prima danneggiare un bersaglio invulnerabile; quando spezza una barriera, la ricarica rimane consumata. Rimuove inoltre le rune nemiche nella piccola zona interessata, senza curare la corruzione.

Rapidità aumenta la velocità del 20 per cento; Forza aumenta i danni del 20 per cento; Vigore riduce del 20 per cento il danno ricevuto. Luce per gli umani e Furia abissale per gli oscuri aggiungono il 15 per cento ai danni. Non esiste ancora una separata categoria numerica di danni sacri o oscuri: sono varianti tematiche del potenziamento. Una selezione multipla consuma una runa per ogni destinatario valido; destinatari incompatibili non vengono addebitati. Una runa non si sovrappone alla trasfigurazione.

La paralisi impedisce movimento, attacco e magia. Le figure speciali di comando hanno durata ridotta a 3 secondi; edifici e navi sono esclusi. Al termine resta immunità per circa 12 secondi, per evitare catene continue. Entrare nella zona protettiva libera una paralisi già attiva. I cerchi e l’indicazione Rituale anticipano il lancio e permettono di reagire.

## Trasfigurazione rischio e guarigione

Scegli un combattente terrestre vicino al Santuario o Altare della Trasfigurazione e usa Trasfigura. Servono 15 cristalli e una distanza inferiore a 360 dalla struttura. Sono esclusi operai, specialisti senza attacco, macchine d’assedio, guastatori, navi e volanti. La forma cambia nell’aspetto della fazione opposta senza trasferire subito la fedeltà.

| Fase | Durata | Cosa succede |
| --- | --- | --- |
| Potere controllato | 25 s | Danni più 40 per cento, velocità più 15 per cento; ordini normali |
| Instabilità | 10 s | Il bonus finisce, ma rimane il rischio indicato sopra l’unità |
| Corruzione | Fino al recupero o alla morte | Unità fuori controllo, ostile a entrambe le fazioni |
| Cura | 18 s presso la struttura | Costo 60 oro e 40 legno; massimo due pazienti per struttura |
| Recupero | 240 s dopo la guarigione | Non può essere trasfigurata di nuovo |

Puoi curare l’unità prima che diventi ostile rinunciando al potenziamento. Per recuperare un corrotto servono due combattenti alleati non trasfigurati entro 160 e una struttura della cura entro 360. Seleziona la struttura, premi Recupera corrotto e clicca sul bersaglio. Il contenimento lo rende trasportabile verso il trattamento tramite il percorso automatico, sospendendone il combattimento; non è un ordine militare normale.

Durante il trattamento l’unità non combatte. Se la struttura viene distrutta, ricompare l’instabilità e bisogna trovare un altro luogo di cura. La struttura può essere ricostruita e il trattamento pagato con oro e legno, senza richiedere nuovi cristalli: l’esaurimento delle caverne non rende automaticamente impossibile il recupero. La morte resta definitiva. Curare presto, caricare un salvataggio o cambiare forma non azzera i 240 secondi di recupero.

## Mare e trasporti volanti

I trasporti navali e le arche volanti hanno sei posti per unità terrestri. Seleziona le truppe e clicca con il destro sul trasporto. Per scendere seleziona il trasporto, usa Sbarca o U e clicca su terreno libero vicino. Le navi richiedono una costa raggiungibile; le arche attraversano gli ostacoli volando ma sbarcano solo su terra libera entro il raggio previsto. Sono lente, costose e vulnerabili alle unità antiaeree.

Fregate e ricognitori rilevano i sommergibili. Corazzate, fregate e sommergibili mantengono ruoli differenti. Gli avversari producono rinforzi navali pagati nelle missioni pertinenti e possono imbarcare soldati reali e tentare sbarchi contro un porto. I rinforzi non compaiono dal nulla. La frequenza e l’efficacia dei nuovi assalti marini richiedono ancora prove prolungate sulle mappe ricostruite.

## Campagna e storia

Le due fazioni condividono ancora la geometria e gli obiettivi principali delle undici mappe. Eserciti, avversari, incantatori e introduzioni si adattano alla scelta. La nuova tecnologia è spiegata nei briefing e diventa disponibile gradualmente; resta utilizzabile liberamente entro i requisiti. Una seconda campagna oscura con mappe e obiettivi autonomi non è inclusa in questa alfa.

| Capitolo | Territorio e insegnamento |
| --- | --- |
| 1 | Valle dei Mulini; economia libera e campo rivale facoltativo di lavoratori |
| 2 | Passo delle Sentinelle; soldati, presidio e pattuglia |
| 3 | Bosco dei Custodi; archivio, arcieri, cristalli e specialisti |
| 4 | Gola delle Braci; cavalleria, cantieri e riparazione |
| 5 | Costa delle Città Libere; petrolio, flotta, trasporto e tempio insulare |
| 6 | Altipiano delle Testimonianze; steli, rune e specialisti di contrasto |
| 7 | Valle delle Fornaci; produzione, assedio e risorse contendibili |
| 8 | Arcipelago delle Chiuse; canali, faro, protezione dei porti e sbarchi |
| 9 | Terrazze della Memoria; Comando III, barriere, vincoli e trasfigurazione |
| 10 | Fronte dell’Alba; nuovi reparti e recupero dei presidi |
| Finale | Cuore del Mondo; Comando IV, arche e draghi, distruzione di tutte le forze nemiche |

Liora e Vaerak apprendono che i cristalli sono condensazioni limitate delle correnti regolate dai sigilli. Le steli le incanalano; le rune fissano un disegno temporaneo; le barriere sostengono quel disegno. Spezzarlo non distrugge automaticamente la Pietra. La trasfigurazione altera la forma e mette a rischio la volontà, senza dimostrare che una delle stirpi sia una malattia. La cura è stabilizzazione per entrambe le parti.

Gli angeli minori rappresentano manifestazioni limitate dei doni e della rete dei santuari, non il ritorno dei Cinque Creatori. Le arche sono scafi fantasy sostenuti da rune e cristalli, senza tecnologia fantascientifica. Questa integrazione accompagna la campagna esistente: non è un seguito narrativo completo già messo in scena. La vittoria finale conserva la Pietra e richiede la sconfitta di ogni nemico e struttura avversaria.

## Grafica e animazioni realizzate

Sono stati rivisti cammino, orientamento, attrezzi, raccolta, trasporto, attacchi e cantieri. Gli umani conservano martello e piccone in legno e metallo; gli imp hanno martello infernale e piccone ricurvo d’ossidiana. I carichi sono tronchetti legati, sacchi d’oro e cristalli. La camminata segue lo spostamento e si ferma quando il personaggio è fermo.

Un nuovo atlante originale contiene 36 celle con strutture e unità delle due fazioni, comprese alcune varianti di confronto non reclutabili. È stato creato con imagegen; file e prompt sono in progetto/assets/abisso-v2. I fotogrammi direzionali completi delle nuove cavalcature e di tutti i personaggi non sono ancora stati disegnati: diverse animazioni rimangono procedurali. Anche le quattro fasi architettoniche complete di ogni struttura restano una rifinitura artistica futura.

## Salvataggi e verifiche

I nuovi mondi usano il formato 6, che registra cristalli, ruoli, rune, barriere, paralisi, rituali, trasformazione e recupero. I formati precedenti restano leggibili. Le anteprime con layout ricostruito in formato 5 ricevono le caverne durante il caricamento; salvataggi più vecchi mantengono il proprio layout storico. Per una prova completa è consigliata una nuova campagna. Non aprire un salvataggio 6 con un eseguibile precedente che non lo supporta.

Le verifiche automatiche coprono economia e produzione, risorse finite, popolazione, percorsi, mappe, condizioni di missione, salvataggi e comandi dell’interfaccia. La suite specifica 2.0 controlla anche corruzione recuperabile, ricariche persistenti, proiettili dei rinnegati, dissoluzione, protezione dalla paralisi e imbarco aereo. Le prove militari complete con economia pagata conservano lo scenario storico; non dimostrano da sole il bilanciamento del nuovo finale.

Le immagini di verifica delle nuove fazioni sono allestimenti controllati nel motore, con risorse e strutture predisposte. Non sono presentate come partite vinte normalmente. I risultati definitivi sono nel rapporto VERIFICA-ALPHA1.json, aggiornato alla revisione verificata.

## Cosa resta da rifinire

Prima di una release stabile restano prove giocate complete delle undici mappe con i nuovi eserciti, bilanciamento delle asimmetrie, frequenza dei raid e degli sbarchi, misure di fluidità su PC reali, revisione delle traduzioni nel contesto, eventuali difetti di selezione e percorso e completamento delle animazioni e delle varianti architettoniche. I nuovi artwork vanno controllati anche a tutte le distanze di zoom. Audio, multiplayer, editor di mappe e campagna oscura autonoma non fanno parte di questa build.

Per segnalare un problema annota versione, fazione, missione e azione che lo provoca. Se possibile conserva il salvataggio e uno screenshot. Le prime prove consigliate sono una raccolta fino all’esaurimento, un edificio al IV, una pattuglia, un rituale interrotto, una barriera spezzata, una cura anticipata e il recupero di un corrotto. Poi una partita navale e il finale con ciascuna fazione.

## Riferimenti per gli sviluppatori

Il motore è C++17 con SFML 2.5.1. RtsCatalog.h definisce ruoli e costi; RtsV2.inl implementa le nuove regole; RtsWorld.inl integra economia e combattimento; RtsSave.inl gestisce i formati; GameplayCommands.inl e GameplayRenderer.inl collegano comandi e presentazione. v2_systems_tests.cpp verifica le interazioni; catalog_export.cpp rende riproducibile l’inventario usato in questa guida.

Per compilare usare strumenti/Compila.ps1. Per le verifiche usare strumenti/Verifica.ps1, oppure l’opzione SoloSimulazione senza il contesto grafico. I log e i profili di prova restano separati dai dati del giocatore. Sorgenti, inventario, registro delle modifiche e documenti sono destinati al ramo develop/2.0; main, release/1.x e il tag v1.0.0 rimangono invariati.

### Registro sintetico dello sviluppo

| Incremento | Contenuto |
| --- | --- |
| V2 001 | Griglie dinamiche, mappe grandi e profili di anteprima separati |
| V2 002 | Crescita progressiva delle regioni e reclutamento entro i posti liberi |
| V2 003 | Ricostruzione di tutte le mappe, percorsi A*, punti di raccolta e campo rivale opzionale |
| V2 004 | Riempimento trasparente degli edifici e revisione di cammino e attacchi |
| V2 005 | Pulizia degli indicatori e miglioramento degli utensili e del cantiere |
| V2 006 | Carichi di legna e oro più rifiniti e articolazione del passo |
| V2 007 | Martello e piccone distinti per la stirpe oscura |
| V2 008 | Cristalli, specialisti, livelli IV, ruoli, nuovi eserciti, rune, barriere e trasfigurazione |

Le versioni storiche della guida di design e i rapporti precedenti rimangono nel progetto per ricostruire le decisioni. Il numero nel nome di un vecchio documento narrativo indica la revisione del testo, non necessariamente la versione dell’eseguibile. Per i numeri effettivi della build corrente prevalgono il catalogo esportato, questa guida e il rapporto di verifica associato.

Il formato di salvataggio conserva esplicitamente i campi e valida dimensioni, valori e identificativi prima di sostituire la partita attiva. Il caricamento fallito conserva il mondo corrente. Le durate dei potenziamenti e delle cure sono tempi della simulazione: la pausa li ferma; ricaricare la partita ripristina il tempo rimasto anziché concedere un nuovo effetto.
