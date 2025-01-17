#import "./template/template.typ": *
#import "@preview/fletcher:0.5.4" as fletcher: diagram, node, edge
#set math.equation(numbering: "(1)")

#show: template.with(
  meta: (
    title: "Inteligentní logická sonda pro výukové účely",
    author: (
      name: "Milan Jiříček",
      email: "jiricmi1@fel.cvut.cz",
      url: "https://github.com/jiricmi/logic_probe",
    ),
    
    submission-date: datetime(year: 2025, month: 1, day: 19),
    bachelor: true,
    
    supervisor: "doc. Ing. Jan Fischer, CSc.",
    faculty: "Fakulta elektrotechnická",
    department: "Katedra měření",
    study-programme: "Otevřená informatika",
    branch-of-study: "Computer Engineering", // studijni obor
  ),
  
  print: false,

  //abstract-en: [
    
 // ],

 // abstract-cz: [
    
 // ],

  acknowledgement: [
    Rád bych tímto poděkoval panu doc. Ing. Janu Fischerovi, CSc., za jeho cenné rady, odbornou pomoc a ochotu sdílet své znalosti. Děkuji mu také za jeho čas a podnětné připomínky.
    
  ],
)

// add pwsh syntax highlighting and make it the default lang for blocks
#show raw.where(block: true): set raw(syntaxes: "./res/PowerShell.sublime-syntax")
#show raw.where(block: true): set raw(lang: "powershell")

= Úvod
Vzdělávání v oblasti elektrotechniky a elektroniky vyžaduje nejen hluboké teoretické znalosti, ale také praktické dovednosti a umění si poradit s naskytnutým problémem. Pro řešení nejrůznějších překážek při navrhování elektronických obvodů je podstatné vědět, jak používat specializované nástroje, které umožňují chybu odhalit. Nástroje jsou také důležité ve školách, kde student může pomocí praktických příkladů zjistit teoretické zákonitosti. Nástroje studentům pomáhají pochopit chování a vlastnosti elektronických obvodů v realném světě. Jedním z těchto nástrojů je logická sonda, zařízení používané k analýze digitálních signálů a diagnostice obvodů. Aby nástroj přispěl ke vzdělávání, je žádoucí aby takové zařízení bylo intuitivní, multifunkční a přizpůsobené náležitostem laboratorní výuky.

Tato semestrální práce se zaměřuje počáteční návrh a realizaci logické songy, která bude navržena a optimalizována pro využití ve výuce středoškolských oborů či ve výuce vysokoškolských oborů. Cílem je vytvořit zařízení, které umožní méně zkušeným studentům provádět klíčové diagnostické úkony, jako například, nastavování úrovní signálů, odchytávání signálů, měření frekvence, měření napětí, generování signálů a další. Sonda bude navržena s důrazem na jednoduché ovládání a to prostřednictvím UART, aby byla snadno použitelná i pro studenty, kteří se s elektronikou setkávají poprvé.

V této práci budou představeny požadavky na zařízení a realizace této logické sondy.


= Teoretické základy
== STM32G030
Pro návrh v této semestrální práci byl zvolen mikrořadič *STM32G030* od firmy STMicroelectronics. @STM32G0-Series Tento mikrořadič je vhodný pro aplikace s nízkou spotřebou. Je postavený na 32bitovém jádře ARM Cortex-M0+, které je energeticky efektivní a nabízí dostatečný výkon pro běžné vestavné aplikace. Obsahuje 32 KiB  flash paměť a 8 KB SRAM @STM32G0-REF.

Pro řadu G030 jsou typické kompaktní rozměry ať už vývojové Nucleo desky, tak typové pouzdra jako například TSSOP20 nebo QFN32, což poskytuje snadnou integraci do kompatního hardwarového návrhu @STM32G030x6-tsop.

=== Analogo-digitální převodník <adc>
Mikrokontrolér STM32G030 je vybaven ADC, který obsahuje 8~analogových kanálů o~rozlišení 12 bitů. Toto rozlišení poskytuje vysokou přesnost při měření napětí a umožňuje detekci i malých změn v signálu. Maximální vzorkovací frekvence převodníku je 1 MSPS#footnote[milion vzorků za sekundu].
#figure(
  caption: "Blokový diagram AD převodníku",
  image("adc-block-diagram.png"),
)
Při měření kanálů se postupuje sekvenčně, která je určená pomocí tzv. ranků. Při požadavku o měření převodník nejprve změří první nastavený kanál, při dalším požadavku druhý a až změří všechny, tak pokračuje opět od počátku.

Aby během měření bylo dosaženo maximální přesnosti, převodník podporuje tzn. oversampling#footnote[Proběhne více měření a následně jsou výsledky např. zprůměrovány aby byla zajištěna větší přesnost.]. Převodník obsahuje *accumulation data register*, který akumuluje měření a~poté pomocí data shifteru vydělí počtem cyklu @STM32G0-ADC.
$ "měření" = 1/M × sum_(n=0)^(n=N-1) "Konverze"(t_n) $


AD převodník, po dokončení měření vzorků, vrací hodnotu, která není napětí. Pro převedení hodnoty převodníku na napětí je nutné znát referenční napětí systému ($V_("REF+")$). Referenční napětí může být proměnlivé, hlavně pokud systém využívá  $"VDDA"$#footnote[VDDA je označení pro analogové napájecí napětí v mikrokontrolérech STM32.] jako referenci, která může kolísat vlivem napájení a  nebo zatížení.

Pro výpočet $V_("REF+")$ se používá interní referenční napětí $V_("REFINT")$ kalibrační data uložená během výroby mikrořadiče a naměřené hodnoty z ADC @STM32G0-REF.

Vztah pro výpočet je následující:
$ V_("REF+") = (V_("REFINT_CAL")×3000)/V_("REFINT_ADC_DATA") $ <vref>
kde:
- $V_("REFINT_CAL")$ je kalibrační hodnota interního referenčního napětí, která je uložená ve~flash paměti mikrořadiče během výroby. Tato hodnota představuje digitální hodnotu, kdy $V_("REF+")$ je přesně $3.0$ $V$. Hodnota se získává čtením z pevné adresy#footnote("Např. u STM32G0 je adresa kalibrační hodnoty: 0x1FFF75AA")@STM32G0-REF @VREF_STACKOVERFLOW.
- 3000 je konstanta odpovídající referenčnímu napětí při kalibraci vyjádřená v milivoltech.
- $V_("REFINT_ADC_DATA")$ je aktuální naměřená hodnota na AD převodníku.

Po zjištění referenčního napětí dle @vref, lze získat na základě referenčního napětí, velikosti převodníku a hodnoty naměřené převodníkem, dle @V_ch. Rozlišení v~případě tohoto zařízení bude 12 bitů. Počet bitů je podstatný pro určení, jaká hodnota je maximální, neboli referenční napětí.
$ V_("CH") = V_("CH_ADC_DATA")/(2^("rozlišení") - 1) × V_("REF+") $ <V_ch>
kde:
- $V_("CH")$ je napětí naměřené na daném kanálu.
- $V_("CH_ADC_DATA")$ je digitální hodnota získaná z AD převodníku.
- $"rozlišení"$ je počet bitů AD převodníku.
- $V_("REF+")$ je referenční hodnota napětí.
=== Časovače  <timery>
STM32G0 obsahuje několik časovačů, které se dají využít pro logickou sondu. Mikrořadič má zabudovaných několik základních#footnote[Basic Timers] a jeden advanced timer#footnote[Advanced Timers]. Základní timery jsou 16~bitové a jsou vhodné pro měření doby či generování jednoduchých PWM signálů. Pokročilý časovač je na tomto mikrokontroleru 32bitový a poskytuje více kanálů. Tyto časovače také podporují nejen generování signálů na výstup, ale také zachytávání signálů a měření délky pulzů externího signálu. Pokročilý časovač nabízí řadu nastavení např. nastavování mezi normálním a inverzním výstupem PWM, generovat přerušení při dosažení specifické hodnoty časovače a podobně @TIMERS.

Časovače jsou obecně velice komplexní téma. Tato práce se bude soustředit pouze na~potřebnou část.

Před spuštěním časovače je potřeba nastavit, jak často má časovač čítat. Frekvenci časovače nastavuje tzn. prescaler, neboli "předdělička". Prescaler dělí s konstantou, která je zvolena, frekvenci hodin dané periferie. Pro případ STM32G0 je to $64$ $"MHz"$#footnote[Frekvenci hodin je možné upravit například pomocí CUBE IDE.]. Frekvence časovače určuje, jak často časovač inkrementuje svou hodnotu za jednu sekundu @STM32G0-REF.
$ F_("TIMx") = F_("clk")/"Prescaler + 1" $

Velikost čítače časovače, zda je 16bitový nebo 32bitový#footnote[U 16 bitového časovače je maximální perioda 65535.], souvisí s jeho tzv. periodou. Perioda určuje hodnotu, při jejímž dosažení se čítač automaticky resetuje na 0. Tuto hodnotu lze nastavit podle potřeby vývojáře. V kombinaci s prescalerem lze nastavit konkrétní časový interval, který je požadován. Časový interval lze vypočítat @timer-int.
$ T = (("Prescaler" + 1) × ("Perioda" + 1) )/ F_("clk") $ <timer-int>

=== USART<usart>
Universal Synchronous Asynchronous Receiver Transmiter je flexibilní periferie, která umožňuje seriovou komunikaci jak v asynchroním tak v synchroním režimu.
Pro tuto aplikaci je využíván UART, kde data jsou odesílána bez společného hodinového signálu mezi odesílatelem a přijemcem. Místo toho je podstatný baudrate#footnote[Rychlost přenosu], což určuje počet přenesených bitů za sekundu. UART podporuje nastavení různých protokolů komunikace jako například RS-232 a R-485. UART také umí full duplex komunikaci @USART-REF.

Data jsou přenášena v tzv. rámcích, které jsou strukturovány následovně:
- *Start bit* - Každý rámec začíná start bitem, který určuje začátek rámce. Bit je vždy "0".
- *Slovo dat* - Poté následuje 8 bitů dat#footnote[Lze používat i 7 bitů nebo  9 bitů dat.].
- *Paritní bity* - Paritní bity slouží k detekci chyby v přenosu. Parita nám dokáže pouze detekovat chybu rámce pouze v případech, kdy nevznikne chyb více#footnote[Chyba z dat 1101 na 1000 nelze detekovat, protože lichá parita má paritní bit v obou případech 0.].
- *Stop bit* - Stop bit#footnote[Stop bitů může být i několik.] signalizuje konec přenosu rámce. Obvykle logická "1".
Práce využívá periferii USART2, protože je na většině vývojových desek řady STM32, konkrétně STM32G0, připojena k rozhraní STLink, což umožňuje komunikaci přes USB konektor. Díky této konfiguraci je možné jednoduše realizovat sériovou komunikaci prostřednictvím připojení k počítači přes USB pro vývoj @USART-REF.
== Ansi sekvence
Ansi escape codes jsou speciální kódy používané pro formátování textu v terminálech, které podporují ANSI standard. ANSI kódy poskytují změnu vzhledu textu, jako je barva pozadí, písma, pozicování a další. Největší využití mají ve vývoji terminálových rozhraní zvaná TUI.
=== Historie
Sekvence vnikly jako standardizovaný soubor kódů, který měl za úkol sjednotit různé značky a modely terminálů, které používaly vendor-specific#footnote[Nestandardatizované kódy, který si každá společnost navrhla sama.] metody, pro tvorbu TUI. Nejznámější terminál, který podporoval ANSI sekvence byl Digital VT100. Jelikož byl velice populární, většina nových terminálů se začaly chovat podle ANSI#footnote[Název ANSI byl vytvořen až později. Odpovídá zkratce American National Standards Institute.] sekvencí. Následně tyto sekvence byly standardizovány @WIKI-ANSI.

=== Kódy
Escape kódy začínají *ESC*#footnote[\\33] znakem, následovným *[*, který značí začátek sekvence, a poté symboly, které určují efekt a celá sekvence je zakončena písmenem.#footnote[Existují také ESC N nebo ESC \\ apod.ale tyto se téměř nepoužívají.]

#v(5pt)
```bash
  ESC [ <parametry> <akce>
```
#v(5pt)
Pro změnu barvy a obecně textu je použito písmeno *m* jako akce.
Nejčastější kódy jsou následující:
- Změna barvy textu
  - 30 až 37: Základní barvy
  - 90 až 97: Světlé verze barev
  - 40 až 47: Základní barvy pozadí
  - 100 až 107: Světlé verze barev
- Textové efekty
  - 0: Reset předchozích efektů
  - 1: Tučný text
  - 4: Podtržení
  - 7: Inverzní
  - 9: Přeškrtnutý

Sekvence také lze použít pro pohyb kurzoru, což je užitečné pro vizuál aplikace @GITHUB-ANSI.
#v(5pt)
```bash
ESC[<row>;<col>H // Pohyb na konkrétní pozici
ESC[<posun><směr> // Posune o danou pozici
```
#v(5pt)
== STM HAL
Hardware abstraction layer je knihovna poskytovaná společností STMicroelectronics pro jejich mikrořadiče řady STM32. Tato knihovna tvoří vrstvu abstrakce mezi aplikací a~periferiemi mikrokontroléru. Pokytuje funkce na vyšší úrovni, které usnadňují přístup např. k GPIO, USART, SPI, I2C bez nutnosti přímého přístupu k registrům procesoru @STM-CUBE.

Mezi vlastnosti, kromě zmíněné jednoduchosti patří přenositelnost. Spousta mikrořadičů například využívají jiné adresy pro specifickou funkcionalitu. Pokud vývojář bude potřebovat portovat aplikaci na jiný mikrořadič, není nutné přepisovat různé adresy a logiku programu ale pouze změnit hardware a jelikož program pracuje s abstrakcí, bude nadále fungovat.ti přímého přístupu k registrům procesoru. Na @stm32cubemx-arch je znázorněn diagram, který znázorňuje architekturu HAL @STM-HAL-ARCH.

Součástí HALu je tzv. CMSIS#footnote[Cortex Microcontroller Software Interface Standard], což je sada standardizovanách rozhraní, které umožňují konfiguraci periferií, správu procesorového jádra, obsluhu přerušení a další @ARM-CMSIS.

CMSIS je rozdělen do modulárních komponent, kdy vývojář může využít pouze části, které potřebuje. Např. CMSIS-CORE, která poskytuje přístup k jádru Cortex-M a periferiím procesoru, obsahuje definice registrů, přístup k NVIC#footnote[Nested Vectored Interrupt Controller] apod. @ARM-CMSIS

Hlavní rozdíl mezi CMSIS a HALu#footnote[STMicroelectronics do svého HALu zabaluje i CMSIS od ARM.] STMicroelectronics je ten, že CMSIS je poskytnuto přímo ARM a slouží pouze na ovládání Cortex M procesorů zatímco část od STMicroelectronics poskytuje abstrakci periferií. 


#figure( caption: "STM32CubeMX HAL architektura",
  image("hal-architecture.png")
) <stm32cubemx-arch>

= Realizace
== Grafické rozhraní
Pro snadné pochopení ovládání i jedincem, který se zabývá podobným tématem poprvé, je podstatné, aby logická sonda byla jednoduše ovladatelná, přenositelná a obecně aby zprovoznění sondy nebylo náročné. Pro spoustu začátečníků může být obtížné zjistit, jak mikrokontrolér připojit k PC, jaký program nainstalovat, jaké ovladače nainstalovat a~podobně.

Proto v případě logické sondy bylo zvoleno řešení, kde uživatel pouze připojí logickou sondu k PC a nainstaluje si známý terminál pro seriovou komunikaci, a může sondu používat. Je to z důvodu, že logická sonda využívá ANSI escape sekvence pro generování terminálového uživatelského rozhraní. Tento přístup nevyžaduje instalaci ovladačů pro specifický software a hlavně není závislý na operačním systému. Tzn. podpora této logické sondy je na všechny standartní operační systémy. V @usart bylo zmíněno, že UART umí full duplex komunikaci, díky tomu počítač může posílat zprávy i do mikrokontroleru. Tuto vlastnost logická sonda použije pro ovládání rozhraní uživatelem.

V rámci této práce byl využíván software *PuTTY*. FOSS#footnote[Free open source software], který má podporu různých komunikačních protokolů, jako je SSH, Telnet, SCP a další. PuTTY podporuje také ANSI escape sekvence a je možné upravit velké množství nastavení.

=== Odesílání zpráv <ansi-send>

Před odesláním první zprávy přes seriovou komunikaci je žádoucí inicializovat UART periferii. To je možné přes STM32CubeMX#footnote[STM32CubeMX je grafický program, který dává jednodušší možnost úpravy periferií.], kde vývojář nastaví potřebné parametry a je mu vygenerován základní kód. Pro potřeby projektu bylo zvoleno následující nastavení:
#v(10pt)
```C
static void MX_USART2_UART_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B; // velikost dat
    huart2.Init.StopBits = UART_STOPBITS_1; // počet stop bitů
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX; // Zapnut full duplex
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }
}```
#v(10pt)
Po inicializaci je možné poslat zprávu pomocí například následovně:
#v(10pt)
```C
void ansi_send_string(const char* str) {
    HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}
```
#v(10pt)
kde reference na `huart2` je inicializovaná struktura.
Pro jednodušší generování rozhraní byla vytvořena abstrakce zvaná `ansi abstraction layer` (dále jen AAL). Tato abstrakce umí potřebné ansi senkvence generovat. Zde je příklad funkce, která odesílá text, který již má speciální efekty:
#v(10pt)
```C
// Odesílání textu s efekty
void ansi_send_text(const char* str,
                    const char* color,
                    const char* bg_color,
                    const _Bool bold) {
    if (bg_color != NULL && strlen(bg_color) != 0) {
        ansi_send_string(bg_color);
    }
    if (color != NULL && strlen(color) != 0) {
        ansi_send_string(color);
    }
    if (bold) {
        ansi_send_string("\033[1m"); // Tučné písmo
    }
    ansi_send_string(str);
    ansi_send_string("\033[0m"); // Reset formátování
}

```
#v(10pt)
Nebo například funkce, která nastavuje kurzor na specifickou pozici: 
#v(10pt)
```C
void ansi_set_cursor(const uint8_t row, const uint8_t col) {
    if (row > TERMINAL_HEIGHT || col > TERMINAL_WIDTH) {
        Error_Handler();
    }

    char result[BUFF_SIZE];

    size_t ret = snprintf(result, BUFF_SIZE, "\033[%u;%uH", row, col);

    if (ret >= sizeof(result) || ret < 0) {
        Error_Handler(); // kontrola zda se text vešel do bufferu
    }

    ansi_send_string(result);
}```
#v(10pt)
Další funkce, které zjednodušují ovládání jsou k nalezení v souboru `ansi_abstraction_layer.c`. Pro další kontext budou postačovat hlavně tyto dvě funkce.

Pomocí AAL je možné vykreslovat efektivně větší celky. Zde je příklad z TUI, kdy jedna funkce pomocí instrukcí sestaví větší celek, v tomto případě nápovědu pro odchytávání signálu: 
#v(10pt)
```C
void ansi_frequency_reader_generate_hint(void) {
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 4);
    ansi_send_text("m - change mode ", RED_TEXT, "", false);
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 21);
    ansi_send_text("t - change sample time ", BLUE_TEXT, "", false);
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 45);
    ansi_send_text("d - delete flag ", GREEN_TEXT, "", false);
}

```
#v(10pt)

Tyto větší celky ulehčili tvorbu tzv. `ASCII ART`#footnote[ASCII ART je termín pro obrázek, který je vytvořen pomocí symbolů ASCII.], ohraničení nebo tvorby menu, což vylepší vizuál stránky. @ANSI-PAGE-MAIN ukazuje, jak vypadá hlavní stránka logické sondy realizované skrze AAL.
#figure(
  caption:"TUI hlavní stránky logické sondy",
  image("ANSI_PAGE_MAIN.png"),
)<ANSI-PAGE-MAIN>
#pagebreak()
=== Přijimání zpráv
Pro zajištění komunikace mezi sondou a zařízením je nutné také  zpracovávat vstupy od uživatele, které jsou přijímány na rozhraní UART. K tomuto účelu sonda využívá přerušení, které je vyvolané při přijetí znaku na UART. Pro implementaci je použit callback `HAL_UART_RxCpltCallback`, který je již deklarován v knihovně HAL, tzn. stačí ho definovat pro správnou funkcionalitu.
#v(10pt)
```C
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART2) {
        HAL_UART_Receive_IT(&huart2, &received_char, 1);
        if (received_char == 'r' || received_char == 'R') {  // reload
            ansi_clear_terminal();
            render_current_page();
        } else {
            get_current_control();
        }
    }
}
```
#v(10pt)
Callback se skládá ze dvou částí, jedna je samotný callback, který zjistí, jaký UART přijmul znak a zkontroluje, zda to není znak, který je obecný pro všechny okna TUI. Pokud ne, zavolá `get_current_control`. Kde se nachází switch, který podle stránky, na které se uživatel nachází, zvolí ovládací funkci.
#v(10pt)
```C
void control_main_page(void) {
    switch (received_char) {
        case 'v':
        case 'V':
            ansi_clear_terminal();
            ansi_voltage_page();
            break;
        case 'c':
        case 'C':
            ansi_clear_terminal();
            ansi_channel_set_page();
            break;
        case 'f':
        case 'F':
            ansi_clear_terminal();
            ansi_frequency_reader_page();
            break;
        case 'g':
        case 'G':
            ansi_clear_terminal();
            ansi_impulse_generator_page();
    }
}
```
#v(10pt)
Zde je možné například vidět funkci, která slouží pro ovládání  hlavní stránky. Jak je vidět, funkce podporuje velké i malé písmo. Po zjištění znaku spustí určitou akci. Například při stisku `V`, vyčistí terminalové okno a přenastaví stránku na voltmetr.

Zde je podstatné, že samotný interupt nenastavuje okno, ale pouze přenastaví flag a ke změně dojde v hlavní smyčce. Pokud  by to tak nebylo, mohlo by dojít ke kolizi akcí. Například hlavní smyčka by byla v průběhu překreslování a v ten moment, by bylo vyvoláno přerušení UARTem. Přerušení by překreslilo stránku a program by po přerušení pokračoval ve stejném bodě. Zde by se mohlo stát, že by se polovina stránky překreslila a polovina ne. 

== Měření napětí a zjišťování logické úrovně <volt-measure>
Pro měření napětí je využíván AD převodník. Jak již bylo uvedeno v @adc, převodník po realizaci měření vrátí digitální hodnotu, kterou je potřeba převést na napětí. Pro získání přesného napětí je žádoucí změřit a vypočítat referenční napětí a nepoužívat odhad. V projektech, které se nezakládají na přesnosti je časté používat hodnotu `3.3 V`, což je idealizované napájecí napětí. Ve skutečnosti, je ale běžné, že napětí kolísá, popř. napětí může poklesnout, pokud je mikrořadič vytížen. Pokud je nutné, aby napětí bylo, co nepřesnější, musí být vypočteno realné referenční napětí.

Podstatné je, aby byl inicializován AD převodník. Pomocí HALu je to možné následovně:
#v(10pt)
```C
static void MX_ADC1_Init(void) {
    ADC_ChannelConfTypeDef sConfig = {0};

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1; // předdělička
    hadc1.Init.Resolution = ADC_RESOLUTION_12B; // rozlišení 12 bitů
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT; 
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.LowPowerAutoPowerOff = DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE; // pokračuje i po konci cyklu
    hadc1.Init.NbrOfConversion = 1; // počet kanálů
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
    hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
    hadc1.Init.OversamplingMode = DISABLE;
    hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    adc1_ch = create_adc_channels(&hadc1);
    realloc_v_measures(adc1_ch, &v_measures);
    setup_adc_channels(&hadc1, adc1_ch, true);
}
```
#v(10pt)
kde je spousta nastavení, které pro tento projekt nejsou úplně podstatné nicméně některá konfigurace je stěžejní. 

Parametr `hadc1.Init.NbrOfConversion` určuje, kolik měření bude provedeno, než se AD převodník zastaví. Měření může probíhat na více kanálech (V tomto případě až na 4 kanálech) ale jeden převodník nemůže měřit všechny kanály najednou. Je nutné určit pořadí v jakém bude měřit.

#import fletcher.shapes: diamond
#align(center,
  grid(
    columns: 2,
    gutter: 5pt,
    text(size: 7pt,
        diagram(
           label-size: 7pt,
      	  node-stroke: 1pt,
           node((0,0), align(center)[
             Init
           ], corner-radius: 10pt),
           edge("=>", `HAL_ADC_Start`),
      	  node((0,1), align(center)[
      		  Kanál 1
      	  ], shape: diamond),
          edge("=>" , `HAL_ADC_PollForConversion`),
          node((0,2), align(center)[
      		  Kanál 2
      	  ], shape: diamond),
          edge("=>", `HAL_ADC_PollForConversion`),
          node((0,3), align(center)[
      		  Kanál 3
      	  ], shape: diamond),
          edge("=>", `HAL_ADC_PollForConversion`),
          node((0,4), align(center)[
      		  Kanál 4
      	  ], shape: diamond),
          edge("=>", `HAL_ADC_PollForConversion`),
          node((0,5), [Stop], corner-radius: 10pt),
          edge("r,uuuu,l","=>", `HAL_ADC_Start`)
        ),
    ),
    text(size: 7pt,
        diagram(
           label-size: 7pt,
      	  node-stroke: 1pt,
           node((0,0), align(center)[
             Init
           ], corner-radius: 10pt),
           edge("=>", `HAL_ADC_Start`),
      	  node((0,1), align(center)[
      		  Kanál 1
      	  ], shape: diamond),
          edge("=>" , `HAL_ADC_PollForConversion`),
          node((0,2), align(center)[
      		  Kanál 2
      	  ], shape: diamond),
          edge("=>", `HAL_ADC_PollForConversion`),
          node((0,3), align(center)[
      		  Kanál 3
      	  ], shape: diamond),
          edge("=>", `HAL_ADC_PollForConversion`),
          node((0,4), align(center)[
      		  Kanál 4
      	  ], shape: diamond),
          edge("r,uuu,l","=>", `HAL_ADC_PollForConversion`)
        )
    )
  )
)
V levém diagramu je možno vidět, jakým způsobem funguje převodník, pokud *není* `hadc1.Init.ContinuousConvMode` nastaven na `ENABLE`. Při každém zahájení měření kanálu pomocí `HAL_ADC_PollForConversion`, převodník udělá `x` vzorků během 160,5 cyklů#footnote[Tato hodnota je v STM32G0 nejvyšší a zaručuje nám tu největší možnou přesnost. Některé mikrořadiče nabízí i vyšší počet odběrů.]. Po dokončení vzorkování lze zjistit hodnotu funkcí `HAL_ADC_GetValue`. Jakmile AD převodník dokončí konverzi, kanál se nastaví na 2. Při zavolání konverze znovu se již měří na druhém kanálu. Takto sekvenčně převodník pokračuje dokud nedojde k~poslednímu kanálu. Pokud už další kanál nenásleduje, ad převodník zastaví svou činnost a potřeba ho znovu zapnout aby pokračoval.

V pravém diagramu již kontinuální mód zapnutý a měření probíhá neustále dokola. Pokud AD převodník dojde k poslednímu kanálu, začne měřit opět první. K zastavení dojde pouze v případě, že je zavolána funkce `HAL_ADC_Stop`.

Logická sonda pracuje ve smyčce, kdy jednou za určitý krátký úsek zastaví klasické měření kanálů a změří referenční napětí. Tento způsob zajišťuje neustále validní referenci.

Během měření kanálů je nutné, aby nebyl zbytečně zatěžován procesor. Procesor v hlavní smyčce pracuje na vykreslování dat do terminálu a pokud by prováděl měření, mohlo by to ovlivnit rychlost měření napětí a nemuselo by být dosaženo dostatečnému sebrání vzorků pro plovoucí průměr. Toto je vyřešeno pomocí DMA#footnote[DMA je metoda, kdy periferie umí přímo zapisovat nebo číst z paměti. Vyhoda je, že procesor nemusí zasahovat a šetří se zdroje, které můžou být využity jinde.]. HAL aktivuje DMA pro AD převodník pomocí `hadc1.Init.DMAContinuousRequests` nastavené na `ENABLE`. Parametry DMA lze nastavit pomocí STM32CubeMX. DMA je nastaveno jako cirkularní buffer, do kterého se zapisují hodnoty z AD převodníku. Procesor k těmto datům přistoupí, až bude potřebovat. 

Logická sonda umí počet kanálů nastavovat dynamicky, tzn. může měřit pouze 1 kanál a nebo během běhu programu zapnout další dva. Možnost změny počtu kanálů během chodu je problém, jelikož pokud pro DMA je alokována pamět na zásobníku, velikost musí být ideálně statická.

Mějme alokovanou paměť na zásobníku, která bude 8 integerů, tzn. 2 integery pro každý kanál. Pokud jsou zapnuty 1, 2 nebo 4 kanály nenastává žádný problém, protože nakonci se začnou hodnoty ukládat opět od začátku. Takže při opakovaném zápisu nedojde k~"rozjetí" indexů. A jednodušše můžeme říct, že na indexu 6, je vždy kanál 2.
#align(center)[
#diagram($ 1 edge("rrr", ->, #[Zápis 4 kanálů], bend: #30deg) & 2 & 3 & 4 edge("r", ->) & edge("rrr", ->, #[Zápis 4 kanálů], bend: #30deg)5 & 6 & 7 & 8 edge("d,l,l,l,l,l,l,l,u", ->)$)
]
Pokud ale budeme mít aktivovány 3 kanály dojde k problému. Už při první iteraci se nám indexy posunou, což je nežádoucí.
#align(center)[
#diagram($ 1 edge("rr", ->, #[Zápis 3 kanálů], bend: #30deg) & 2 &edge("r", ->) 3 & edge("rr", ->, #[Zápis 3 kanálů], bend: #30deg)4 & 5 & 6edge("r", ->) & 7edge("rr", ->, #[Zápis 3 kanálů], bend: #30deg) & 8 &  1 edge("d,l,l,l,l,l,l,l,u", ->)$)
]
Existuje možnost toto vyřešit pomocí nalezení nejmenšího společného násobku. Pokud, ale hledáme násobek pro 100, 200, 300, 400, tak dojdeme k tomu, že je to naprosté plýtvání pamětí, které si na STM32G0 nemůžeme dovolit.


Tento problém byl vyřešen tak, že paměť je alokována na haldě, takže je dynamicky vytvořená, a realokuje se při změně, tak aby stále odpovídal počet vzorků a nedocházelo k posunu.

Po určitém časovém úseku, procesor zpracuje data z DMA a zprůměruje hodnoty z AD převodníku, následně hodnoty převede dle metodity v @adc a vykreslí na seriovou linku pomocí ANSI sekvencí zmíněné v @ansi-send.

TUI vykresluje hodnoty na každém kanálu a poté vykresluje, zda je logická úroveň vysoká, nízká a nebo je nejasná. @voltmetr ukazuje vizuál stránky pro měření. Je možné pozorovat, že kanál 1 na pinu `A0` měří `0,0 V` a `L` znázorňuje nízkou úroveň. Kanál 2 ukazuje napětí `3,3V` a je to vysoká úroveň. Kanál 3 je plovoucí a není připojený. Proto úroveň je nejasná a měří pouze parazitní napětí. Kanál 4 je vypnutý.

Kanály je možné zapínat a vypínat pomocí stránky `Channels`. @channel ukazuje vzhled této stránky. Uživatel pomocí klávesových zkratek 1 až 4 volí jaké kanaly aktivovat, s tím, že po zvolení kanálů je nutné nastavení uložit stisknutím klávesy S.

Všechny data ohledně kanálů AD převodníku jsou uloženy ve struktuře zvané `adc_channels`. Tato struktura drží, jaké kanály jsou aktivovány, jaké kanály jsou označeny uživatelem, ale ještě nebyly uloženy a tím pádem aplikovány, jaká byla poslední průměrná hodnota měření, jaká čísla pinů kanály osidlují a nakonec instance `ADC_HandleTypeDef`, což je HAL struktura, která je abstrakce ovládání převodníku, ukládání konfigurací apod.

#figure(
  caption: [Stránka pro měření napětí a logických úrovní],
  image("voltmetr-page.png")
)  <voltmetr>
#figure(
  caption: [Stránka pro nastavení jednotlivých kanálů],
  image("channel_page.png")
) <channel>
#v(10pt)
```C
typedef struct {
    _Bool channel[NUM_CHANNELS]; // aktivované kanály
    _Bool channel_unapplied[NUM_CHANNELS]; // kanály neaktivované
    _Bool applied; // bylo nastavení uživatele aplikováno?
    uint32_t avg_last_measure[NUM_CHANNELS]; // poslední průměr hodnot
    unsigned int pin[NUM_CHANNELS]; // čísla pinů
    unsigned int count_active; // počet aktivních pinů
    ADC_HandleTypeDef* hadc; // instance adc pro danou strukturu
} adc_channels;```
#v(10pt)
== Odchytání pulzů a frekvence
Pro měření frekvence hraje stěžejní roli časovač. Jak bylo zmíněno v @timery, časovače umí tzv. input capture. Input capture poskytuje možnost měření časových parametrů vstupního signálu, jako například perioda nebo šířka signálu. Časovač inkrementuje hodnotu o dané frekvenci a v momentě kdy na vstupu je objeví náběžná nebo sestupná hrana, dojde k přerušení a aktuální hodnota čítače se uloží do speciálního registru @TIMERS.

Pro přesné zjistění frekvence musí být kladen důraz na režii. Při odběru dat by nesměl procesor provádět jakoukoliv. Toto se dá realizovat pomocí DMA podobně jako v @volt-measure.

K zjištění frekvence je použita tzv. metoda hradlování. Metoda hradlování využívá periodu vzorkování, která je využita pro spočítání finální frekvence. @frequency_eq uvádí způsob, jak spočítat frekvenci pomocí metody hradlování. Frekvence lze spočítat jako polovinu napočítaných pulzů za časový úsek#footnote[Z důvodu, že časovač v tomto případě měří náběžný i sestupný pulz, je vždy počet pulzů v periodě signálu dvojnásobný, proto je zapotřebí počítat pouze s polovinou.]. K tomuto účelu je použit 32 bitový časovač, aby bylo možné měřit, co největší frekvence.
$ F = (N/2)/T $ <frequency_eq>
Pro měření časového úseku je využit druhý časovač. Tento časovač je nastaven předděličkou tak, aby hodnotu inkrementoval za 1 ms. Uživatel poté pomocí TUI nastavuje periodu na požadovaný úsek.
#v(10pt)
```C
signal_detector.frequency =
  (signal_detector.pulse_count / 2) / (signal_detector.sample_times[signal_detector.sample_time_index] / 1000);
```
#v(10pt)

Po spuštění časovač začne inkrementovat hodnotu a v momentě, kdy časovač přeteče tzn. dosáhne poslední hodnoty periody, časovač vyvolá přerušení. Při přerušení se zastaví časovač pro čítání pulzů a spočítají se potřebné hodnoty.

Logická sonda dle @frequency_eq vypočítá frekvenci. Důvod, proč logická sonda počítá oba pulzy a ne pouze nástupnou nebo sestupnou hranu je ten, že při detekci obou hran dokáž sonda spočítat, jak široké pulzy jsou. Tuto skutečnost je možné využít například pro počítání Duty u PWM signálů.

Níže je možné vidět logiku, která počítá šířku pulzu pro vysokou úroveň. Pokud je čas sestupu signálu větší než vzestupu, není potřeba nic přepočítávat. Pokud čas sestupu je nižší než čas vzestupu, znamená to, že časovač přetekl a je nutné od `0xFFFFFFFF` odečíst čas vzestupu. Nakonec sonda přepočítá hodnotu časovače na čas, tzn. vynásobí konstantou, aby byl vzat potaz na frekvenci procesoru.
#v(10pt)
```C
uint32_t rise_pulse_ticks;
if (sig_high_end > sig_high_start) {
    rise_pulse_ticks = sig_high_end - sig_high_start;         
} else if (sig_high_end < sig_high_start) {
    rise_pulse_ticks = (0xFFFFFFFF - sig_high_start) + sig_high_end;
} else {
    rise_pulse_ticks = 0;
}
float high_width = (rise_pulse_ticks * CONST_FREQ) / 1000;
```
#v(10pt)
#figure(
  caption: [Stránka pro měření frekvence a PWM],
  image("detector_freq.png")
)
Duty time je poté spočítaný jako poměr času vysokého signálu a nízkého signálu uvedený v procentech.

Kromě frekvence, umí sonda odchytávat pulzy, jak nízké, tak vysoké. Mezi módy je možné přepínat klávesou. Pro zachytávání signálu je časovač opět nastaven v režimu input capture. Časovač je spuštěn a nyní není využito DMA, ale časovač vyvolává přerušení, pokud dojde k zachycení signálu, toto přerušení zkontroluje, zda hrana signálu je nástupná nebo sestupná. Poté rozhodne podle módu uživatele, kterou hranu má ignorovat a kterou má detekovat. 
#figure(
    caption: [Stránka pro hledání pulzů],
  image("detector_pulse.png")
)
Když časovač odchytí správnou hranu, přerušení nastaví flag, který reprezentuje nalezený pulz. Tento pulz se poté promítne uživateli do TUI. Flag je možné smazat a čekat na další vyvolání přerušení. Do budoucna je plán, přidat automatické smazání flagu po určité době, aby této funkcionality mohlo být využíváno i bez nutnosti připojení na seriovou linku. Po nalezení pulzu se rozsvítí led, která po například 1 sekundě opět zhasne.
#pagebreak()
== Generování pulzů
Při generování pulzu má uživatel možnost nastavit konkrétní délku pulzi prostřednictvím TUI. Nastavená délka určuje, jak dlouho bude pulz trvat. Po nastavení délky může uživatel stisknout příslušnou klávesu, která spustí proces generování pulzu.

Časovač v tomto případě má nastaven prescaler tak, aby se jednodušše počítal čas v~periodě. Poté, co je nastavený časovač, spustí se. Po přetečení časovače se vyvolá přerušení, které značí, že časovač odměřil příslušnou dobu. Přerušení následně časovač zastaví.

Sonda umožňuje generovat pulzy dvou typů. První možností je generace nízké úrovně signálu během trvání vysoké úrovně. Druhou možností je naopak generace vysoké úrovně signálu během nízké úrovně. Uživatel si tedy může zvolit, kterou úroveň chce jako výchozí stav.

Díky těmto režimům je možné i nastavit úrovně, které uživatel nemusí nutně používat jako generování pulzů, ale jako přepínání úrovní dle potřeby.

#v(10pt)
```C
static void MX_TIM16_Init(void) {
    htim16.Instance = TIM16;
    htim16.Init.Prescaler = 63999; // předdělička
    htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim16.Init.Period = 999; // výchozí hodnota periody
    htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim16.Init.RepetitionCounter = 0;
    htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim16) != HAL_OK) {
        Error_Handler();
    }
    HAL_TIM_Base_Start_IT(&htim16);
    sig_gen_init(&signal_generator);
}```
#v(10pt)
@generator-page ukazuje, jak vypadá prozatimní zhotovení rozhraní pro ovládání generátoru pulzů. Pomocí kláves uvedené v nápovědě je možné měnit šířku pulzu a nebo měnit, jaký mód použít.
#v(10pt)
```C
void sig_gen_toggle_pulse(sig_gen_t* generator, const _Bool con) {
    generator->start = false;

    // nastavení periody, která byla nastavena uživatelem
    __HAL_TIM_SET_AUTORELOAD(&htim16, generator->period - 1);

    if (con && generator->con) {
        HAL_TIM_Base_Stop_IT(&htim16);
        generator->con = false;
    } else {
        generator->con = con;
        HAL_TIM_Base_Start_IT(&htim16);
    }
}```
#v(10pt)

#figure(
  caption: [Stránka pro generování signálu],
  image("generator-pulse.png")
) <generator-page>



= Závěr a zhodnocení
Většina témat této práce, jsou pro mě nová. Na počátku této práce mé znalosti ohledně mikrořadičů byly velice povrchní a pouze na takové úrovni, které mi umožňovaly pracovat s mikrokontrolérem pomocí knihoven a vysoké míry abstrakce. 

Tento projekt, vyžadoval, abych se naučil spoustu nových věcí a principy fungování mikrořadiče nevnímal pouze jako "funkci, která mi vrátí výsledek". Nyní vnímám, že mikrořadič není pouze černá skříňka, ale pod povrchem je to chytře navržený stroj, který dává neskutečné možnosti, jak s ním pracovat. Má cesta v tomto odvětví je stále na začátku, ale jsem zapálen pokračovat a učit se novým věcem.

Tato práce je počátek logické sondy pro střední a vysoké školy, které mají za úkol ulehčit výuku a diagnostiku. Hlavní část této práce pro mě byla zanalyzovat problematiku a najít vhodné řešení. Logická sonda již dnes lze použít na jednoduchou diagnostiku. Logická sonda má funkční TUI, které je realizované pomocí ANSI sekvencí, umí měřit napětí na 4~kanálech, umí určovat frekvenci na vstupu, umí zachytávat krátké pulzy ať jsou nízko úrovňové nebo vysoko úrovňové a dokáže i generovat krátké pulzy. Proto hodnotím výsledek práce jako úspěch.

Tímto ale práce na projektu neskončila. Na projektu plánuji pracovat v rámci bakalářské práce  a rozvinout její potenciál a nakonec i naportovat na další mikrořadiče.



#bibliography("bibliography.bib")
