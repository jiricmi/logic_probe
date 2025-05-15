#import "./template/template/template.typ": *
#import "@preview/fletcher:0.5.4" as fletcher: diagram, node, edge, shapes, 
#import "@preview/codly:1.3.0": *
#import "@preview/codly-languages:0.1.1": *
#show: codly-init.with()
#set math.equation(numbering: "(1)")

#let my_dot_list(body) = [
  #v(5pt)

  #block(breakable: false)[
    #set text(size: 11pt, weight: "bold")
    #set par(first-line-indent: 0pt, leading: 0.8em)
    #body
  ]
]

#let blob(pos, label, tint: white, ..args) = node(
	pos, align(center, label),
	width: 28mm,
	fill: tint.lighten(60%),
	stroke: 1pt + tint.darken(20%),
	corner-radius: 5pt,
	..args,
)

#show: template.with(
  meta: (
    title: "Multifunkční diagnostická logická sonda", author: (
      name: "Milan Jiříček", email: "jiricmi1@fel.cvut.cz", url: "https://github.com/jiricmi/logic_probe",
    ), bachelor: true, diff_usage:false, supervisor: "doc. Ing. Jan Fischer, CSc.", faculty: "Fakulta elektrotechnická", department: "Katedra kybernetiky", study-programme: "Otevřená informatika", study-spec: "Základy umělé inteligence a počítačových věd",
    abbrs: (
        "SOP": "Small Outline Package",
        "TSSOP": "Thin Shrink Small Outline Package",
        "PLL": "Phase Locked Loop",
        "POF": "Point Of Failure",
        "FPGA": "Field-Programmable Gate Array",
        "SRAM": "Static Random Access Memory",
        "ADC": "Analog Digital Converter",
        "MSPS": "Milion Samples Per Second",
        "DMA": "Direct Access Memory",
        "PWM": "Pulse Width Modulation",
        "HAL": "Hardware Abstraction Library",
        "GPIO": "General Purpose Input/Output",
        "I2C": "Inter-Integrated Circuit",
        "SPI": "Serial Peripheral Interface",
        "UART": "Universal Asynchronous Reciever Transmiter",
        "CMSIS": "Cortex Microcontroller Software Interface Standard",
        "NVIC": "Nested Vectored Interrupt Controller",
        "IOT": "Internet Of Things",
        "EEPROM": "Electrically Erasable Programmable Read-Only Memory",
        "MSB": "Most Significant Bit",
        "LSB": "Least Significant Bit",
        "ASCII": "American Standard Code for Information Interchange",
        "TUI": "Terminal User Interface",
        "GUI": "Graphical User Interface",
        "CMOS": "Complementary Metal–Oxide–Semiconductor",
        "MCU": "Microcontroller Unit",
        "SWD": "Serial Wire Debug",
        "FOSS": "Free Open Source Software",
        "SSH": "Secure Shell"
)
  ), print: false, lang: "cs",
  abstract-en: [
    This work presents the design and implementation of a multifunctional diagnostic logic probe for teaching logic circuit and embedded system applications. The device replaces complex and costly commercial laboratory instruments, enabling students to measure basic parameters (voltage, resistance, frequency), generate pulses, and diagnose communication interfaces (UART, I2C, SPI, Neopixel). The probe is implemented in two variants: an STM32-based version and a simplified version for Raspberry Pi Pico. A local mode with an RGB LED and button provides quick diagnostics, while a terminal mode with an ANSI-based text user interface (TUI) offers advanced functions, including control and data visualization via UART. The design leverages the STM32 HAL and Raspberry Pi C SDK for code portability and minimizes external components, allowing students to assemble the probe easily on a breadboard. The primary contribution lies in accelerating error identification during practical exercises, such as detecting faulty components or incorrect peripheral wiring.
    #par(first-line-indent: 0em, spacing: 2em)[
        *Keywords:* logic probe, STM32, STM32G0, Raspberry Pi Pico, error diagnostics, UART, I2C, SPI, embedded systems, educational tools, ANSI escape sequences
    ]
    #par(first-line-indent: 0em, spacing: 2em)[
        *Title translation:* Multifunctional Diagnostic Logic Probe
    ]
  
  ],
  abstract-cz: [
    Tato práce představuje návrh a realizaci multifunkční diagnostické logické sondy pro výuku práce s logickými obvody a vestavnými systémy. Zařízení nahrazuje komereční laboratorní přístroje, které jsou složité a finančně náročné, a umožňuje studentům měřit základní veličiny (napětí, odpor, frekvenci), generovat pulzy a diagnostikovat komunikační rozhraní (UART, I2C SPI, Neopixel). Sonda je realizována ve dvou variantách: verze na bázi STM32 a zjednodušená pro Raspberry Pi Pico. Lokální režim s RGB LED a tlačítkem slouží pro rychlou diagnostiku, zatímco terminálový režim s ANSI TUI poskytuje pokročilé funkce včetně ovládání a zobrazování hodnot skrze UART periferii. Návrh využívá STM HAL a Raspberry Pi Pico C SDK pro prenositelnost kódu a minimalizuje externí komponenty, aby studenti mohli sondu snadno sestavit na nepájivém kontaktním poli. Hlavním přínosem je urychlení identifikace chyb při praktických cvičeních, jako jsou odhalení vadné součástky nebo nesprávné zapojení periferií.
    #par(first-line-indent: 0em, spacing: 2em)[
        *Klíčová slova:* STM32, STM32G0, Raspberry Pi Pico, logická sonda, UART, I2C, SPI, vzdělávací nástroje, diagnostika chyb, C/C++, ANSI escape sekvence
    ]
  ],
  acknowledgement: [
    Rád bych tímto poděkoval panu doc. Ing. Janu Fischerovi, CSc., mému vedoucímu práce, za jeho cenné rady, odbornou pomoc a ochotu sdílet své znalosti. Děkuji mu také za věnovaný čas, podnětné připomínky a trpělivost, které mi poskytoval během celého procesu tvorby této práce.

    Dále bych chtěl vyjádřit největší poděkování své rodině a mé přítelkyni za jejich neochvějnou podporu, povzbuzení v náročných momentech a pochopení, jež mi dávali najevo po celou dobu mého studia. Bez jejich lásky a motivace by tato práce nevznikla.

   Nemohu opomenout ani své přátele, kteří mi po celou dobu studia pomáhali udržet optimismus, sdíleli se mnou radosti i starosti, a svou přítomností mi vytvářeli potřebný odstup od pracovního vypětí. Jejich přátelství a ochota být mi oporou v osobním životě významně přispěly k tomu, abych mohl tuto práci úspěšně dokončit.
],
)

// add pwsh syntax highlighting and make it the default lang for blocks
#show raw.where(block: true): set raw(syntaxes: "./res/PowerShell.sublime-syntax")
#show raw.where(block: true): set raw(lang: "powershell")

= Úvod
Výuka základů elektroniky vyžaduje nástroje, které studentům umožní experimentovat s realnými logickými obvody a prakticky ověřovat teoretické znalosti. Tradiční diagnostické přístroje, jako osciloskopy nebo logické analyzátory, jsou však pro začínající studenty složité, finančně náročné a nabízejí funkce nad rámec základních výukových potřeb. Během praktických cvičení se studenti často dostávají do situací, kdy například čítač pulzů s výstupem na 7-segmentový displej přestane fungovat, aniž je zřejmá příčina. Mohou váhat, zda chyba spočívá v softwaru (nesprávně naprogramovaném časovači), hardwaru (spálené LED, vadném senzoru) nebo třeba v prohození vodičů UART (Tx/Rx). Bez vhodného nástroje pak tráví hodiny hledáním závady, což demotivuje a zpomaluje výuku. Tento problém vede k potřebě vytvořit jednoduché, multifunkční a dostupné řešení, které by usnadnilo osvojení principů práce s logickými a komunikačními obvody.

Cílem této bakalářské práce je návrh a realizace multifunkční diagnostické logické sondy, která kombinuje funkce logického analyzátoru, generátoru signálů a testeru komunikačních rozhraní typické pro výuku (UART, I2C, SPI, Neopixel). Hlavní inovací řešení je integrace vzdělávacího aspektu do samotného návrhu zařízení - sonda je navržena ve dvou variantách: plnohodnotná verze založená na mikrokontroleru STM32 a omezená verze využívající Raspberry Pi Pico pro naplnění potřeby středních škol, které nejsou specializováné na výuku elektroniky. Hlavní výhodou řešení je minimalizace externích komponent, které umožňují sestavení zařízení na nepájivém kontaktním poli studentem a integrace s PC terminálovou aplikací pro pokročilé funkce bez nutnosti instalace specialních aplikací a ovladačů.

Klíčovými prvky návrhu jsou jednoduché ovládání (lokální režim s RGB LED a tlačítkem, terminálový režim ovládaný skrze terminálovou aplikaci), měření napětí, odporu, frekvence a střídy PWM signálů, generování pulsů a diagnostika komunikačních periferií. Důraz je kladen na open-source přístup, který umožní další rozšiřování a přizpůsobení vzdělávacím potřebám.

= Rozbor problematiky
== Motivace <rozbor-vyuka>
Během laboratorních cvičení zaměřených na logické obvody a vestavné systémy studenti navrhují digitální obvody a programují mikrokontroléry (MCU). Při vývoji však mohou narazit na situaci, kdy jejich řešení úlohy náhle přestane fungovat podle očekávání, aniž by byla zjevná příčina problému. Najít závadu může být velice časově náročné jak pro studenta, tak pro vyučujícího.
 
Při návrhu softwaru pro mikrokontroléry je klíčové průběžně ověřovat funkčnost pomocí pulsů. Studenti tak mohou například zjistit, zda je generován výstupní pulz požadované frekvence, zda obvod správně reaguje na vstupní impuls, nebo zda je signál přenášen přes daný vodič. Praktickým příkladem je vývoj čítače pulzů s výstupem na 7-segmentový displej – zde sonda umožňuje okamžitě validovat, zda software správně zpracovává vstupy a aktualizuje výstup. Studenti při sestavování obvodů také často čelí problémům jako nefunkční komponenty (spálené LED, vadné senzory) nebo chybám v zapojení – například prohození Tx/Rx vodičů UART, chybějící pull-up rezistory na I2C sběrnici, nebo nesoulad s referenčním schématem. Takové chyby vedou k časově náročnému hledání závad. 

Standardní logická sonda je elektronické zařízení sloužící k diagnostice logických obvodů. Pomáhá určovat logické úrovně a detekovat pulsy. Je to jeden ze standardních nástrojů pro elektrotechniky pracující s FPGA, mikrokontrolery či logickými obvody. Výhoda logické sondy je cena pořízení a flexibilita použití. Logická sonda je jedním z prvních nástrojů, který dokáže najít základní problém v digitálním obvodu. Další běžné nástroje pro diagnostiku logických obvodů jsou osciloskop a logický analyzátor. Tyto nástroje jsou vhodné pro diagnostiku např. I2C sběrnice nebo SPI rozhraní, kdy uživatel může vidět konkrétní průběh signálu. Pro výukové účely však mají zásadní nevýhody: Pořizování analyzátorů a osciloskopů může být velice nákladné, jejich ovládání vyžaduje pokročilé znalosti, a student musí nejprve pochopit, jak s přístrojem zacházet. Navíc nabízejí spoustu funkcí, které jsou pro účely výuky nadbytečné a mohou začátečníky dezorientovat.

Multifunkční diagnostická logická sonda (dále jen sonda), která bude navržena v rámci této bakalářské práce má za cíl, minimalizovat zmíněné problémy konvenčních diagnostických nástrojů a obecně zpřístupnit diagnostiku studentům, kteří jsou stále ve fázi učení. Sonda, která bude vyvinuta, přinese levné řešení, které bude obsahovat potřebné funkce pro základní diagnostiku logických obvodů a bude se snažit studentovi zjednodušit celý proces hledání problému v řešení úlohy i bez hlubokých předchozích znalostí s používáním pokročilých diagnostických nástrojů. 

Student si může tak osvojit metodiku debugování od základních kontrol napájení, přes odchytávání pulsů po analýzu komunikačních periferií. Možnost sestrojení sondy na nepájivém kontaktním poli poskytuje flexibilitu vyučujícím vytvořit rychle multifunkční logickou sondu. Jelikož návrh bere zřetel na možnost realizace studentem, je při sestavování použito minimální počet externích součástek. Tímto je možno dosáhnout úspory času na straně vyučujícího, kdy vyučující odkáže na použití sondy při hledání problému. Použití MCU typu STM32 a RP2040 umožňuje transparentnost, a možnost hlubšího pochopení fungování sondy z důvodu velkého množství manuálů a návodů na internetu.

== Požadavky<cil>
V rámci bakalářské práce bude navržena a realizována multifunkční diagnostická logická sonda (dále jen sonda) na platformě STM32. V návrhu sondy je potřeba zohlednit následující klíčové oblasti: jednoduchost ovládání i uživateli, kteří nemají zkušenosti s používáním pokročilých diagnostických nástrojů, rychlá realizovatelnost sondy na nepájivém kontaktním poli a praktičnost ve výuce. Aby nástroj nebylo komplikované sestavit, je nutné aby bylo využito co nejméně externích součástek. Tím je redukován čas sestavení a také je sníženo množství POF.

Firmware a hardware sondy jsou primárně navrženy pro mikrokontrolér STM32G030 v pouzdrech SOP8 a TSSOP20, které díky integrovaným periferiím (UART, GPIO, časovače) umožňují jednoduché sestavení i na nepájivém kontaktním poli. Sonda bude s omezenou verzí realizována i na Raspberry Pi Pico. Sonda bude vybavena tzv. "lokálním režimem" a "terminálovým režimem".

Lokální režim bude sloužit pro rychlou základní diagnostiku obvodů s indikací pomocí RGB LED a ovládání skrze jedno tlačítko. Bude fungovat bez nutnosti připojení zařízení k PC skrze  UART-USB převodník. Tlačítkem bude uživatel přepínat módy, kanály a úrovně. Lokální režim bude mít následující vlastnosti: nastavení úrovní kanálů, odchytávání pulsů, prověření logické úrovně a generace pravidelných pulsů.

Terminálový režim bude poskytovat konkrétní měření veličin logického obvodu a diagnostiku sběrnic. Sonda bude v tomto režimu ovládána odesíláním symbolů za pomocí periferie UART skrze převodník UART-USB. Sonda takto poskytne uživatelské rozhraní, které se vygeneruje na straně mikrokontroleru a zobrazí skrze terminálovou aplikací podporující tzn. ANSI sekvence#footnote[Např. PuTTY, GTKTerm...]. Oproti ovládání prostřednictvím specializované aplikace vyvinuté namíru sondě, tento přístup zajišťuje přenositelnost napříč operačními systémy a nenutí uživatele instalovat další software, což je výhodné zejména na sdílených zařízeních, jako jsou fakultní počítače.

Sonda v tomto režimu bude nabízet funkce základní a pokročilé. Mezi základní funkce patří: detekce logických úrovní, detekce impulsů, určení jejich frekvence, nastavení logických úrovní, generace impulsů, měření napětí a měření odporu. Mezi pokročilé náleží diagnostika sběrnic UART, I2C, SPI a Neopixel. Sběrnice sonda bude pasivně poslouchat nebo aktivně vysílat. Získaná data budou zobrazováná skrze terminálovou aplikaci.

== Volba mikrokontrolerů pro realizaci sondy
=== MCU STM32G031
Pro návrh v této bakalářské práci byl zvolen mikrořadič STM32G031 od firmy
STMicroelectronics @STM32G0-Series. Tento mikrořadič je vhodný pro aplikace s
nízkou spotřebou. Je postavený na 32bitovém jádře ARM Cortex-M0+, které je
energeticky efektivní a nabízí dostatečný výkon pro běžné vestavné aplikace.
Obsahuje 64 KiB flash paměť a 8 KiB SRAM @STM32G0-REF. MCU umožňuje frekvenci až 64 MHz, kterou je možné měnit pomocí PLL @STM32G0-REF @NI_2024. Mikrokontroler nabízí i periferie jako USART, I2C nebo SPI pro rychlou seriovou komunikaci s dalším zařízením nebo senzory. Pro řadu G031 jsou typické kompaktní rozměry ať už vývojové Nucleo desky, tak
typová pouzdra jako například *TSSOP20* nebo *SOP8*, což poskytuje snadnou
integraci do kompatního hardwarového návrhu @STM32G030x6-tsop. Obě zmíněná pouzdra jsou použita pro implementaci logické sondy, o které pojednává #ref(<kap-hw>, supplement: [kapitola]). V rámci realizace je použité MCU *STM32G030*, které je s kompatibilní s návrhem logické sondy.
==== Analogo-digitální převodník <adc>
Mikrokontrolér STM32G031 je vybaven analogo-digitálním převodníkem#footnote[Neboli ADC], který obsahuje 8~analogových kanálů
o~rozlišení 12 bitů. Maximální vzorkovací frekvence převodníku je 2 MSPS. Při měření kanálů se postupuje v sekvenci určené tzv. ranky#footnote[Rank určuje v jakém pořadí je kanál změřen.]. Při požadavku o měření převodník nejprve změří první nastavený kanál, při dalším požadavku druhý a až změří všechny, tak pokračuje opět od počátku. Během měření je nutné zajistit, aby napětí na kanálu nepřekračovalo $3.3$ V protože AD převodník není $5$ V tolerantní.
Ke snížení vstupního šumu během měření převodník podporuje metodu oversampling#footnote[Proběhne více měření a následně jsou výsledky např. zprůměrovány aby byla zajištěna větší přesnost.]. Převodník obsahuje *accumulation data register*, který přičítá každé měření a~poté pomocí data shifteru vydělí počtem cyklu, kde počet cyklů je vždy mocnina dvojky z důvodu složitého dělení na MCU @STM32G0-ADC. Tato metoda zamezuje rušení na kanálu.
#figure(
  caption: [Blokový diagram AD převodníku @STM32G0-ADC], image("pic/adc-block-diagram.png"),
)
$ "měření" = 1/2^m × sum_(n=0)^(n=N-1) "Konverze"(t_n) $

AD převodník, po dokončení měření vzorků, vrací hodnotu, která není napětí. Tato hodnota je poměrná hodnota vůči napájecímu napětí vyjádřena 12 bitově#footnote[Např. hodnota 4095 značí, že naměřené napětí je stejně velké jako napájecí napětí.]. Pro
převedení hodnoty převodníku na napětí je nutné znát referenční napětí systému ($V_("REF+")$).
Referenční napětí může být proměnlivé, hlavně pokud systém využívá $"VDDA"$#footnote[VDDA je označení pro analogové napájecí napětí v mikrokontrolérech STM32.] jako
referenci, která může být $2$ V až $3.6$ V a také může kolísat vlivem napájení nebo zatížení. Pro výpočet $V_("REF+")$ se používá interní referenční napětí $V_("REFINT")$ kalibrační
data uložená během výroby mikrořadiče a naměřené hodnoty z ADC @STM32G0-REF.

Vztah pro výpočet je následující:
$ V_("REF+") = (V_("REFINT_CAL")×3300)/V_("REFINT_ADC_DATA") $ <vref>
kde:
- $V_("REFINT_CAL")$ je kalibrační hodnota interního referenčního napětí, která je
  uložená ve~flash paměti mikrořadiče během výroby. Tato hodnota představuje
  digitální hodnotu, kdy $V_("REF+")$ je přesně `3.3 V`. Hodnota se získává
  čtením z pevné adresy#footnote("Např. u STM32G0 je adresa kalibrační hodnoty: 0x1FFF75AA") @STM32G0-REF @VREF_STACKOVERFLOW.
- 3300 je konstanta odpovídající referenčnímu napětí při kalibraci ve výrobě vyjádřená v
  milivoltech.
- $V_("REFINT_ADC_DATA")$ je aktuální naměřená hodnota na AD převodníku. Tato hodnota závisí na aktuálním napětí na napájení.

Po zjištění referenčního napětí dle @vref, lze získat na základě referenčního
napětí, velikosti převodníku a hodnoty naměřené převodníkem, dle @V_ch.
Rozlišení v~případě tohoto zařízení bude 12 bitů. Počet bitů je podstatný pro
určení, jaká hodnota je maximální, neboli referenční napětí.
$ V_("CH") = V_("CH_ADC_DATA")/(2^("rozlišení") - 1) × V_("REF+") $ <V_ch>
kde:
- $V_("CH")$ je napětí naměřené na daném kanálu.
- $V_("CH_ADC_DATA")$ je digitální hodnota získaná z AD převodníku.
- $"rozlišení"$ je počet bitů AD převodníku.
- $V_("REF+")$ je referenční hodnota napětí.

==== Časovače <timery>
STM32G031 obsahuje několik časovačů (timeru), které se dají využít pro logickou sondu.
Mikrořadič má zabudovaných několik základních a jeden pokročilý časovač. Základní časovače jsou 16~bitové a jsou
vhodné pro měření doby či generování jednoduchých PWM signálů. Pokročilý časovač
je na tomto mikrokontroleru 32bitový a poskytuje více kanálů. Tyto časovače také
podporují nejen generování signálů na výstup, ale také zachytávání signálů a
měření délky pulzů externího signálu. Pokročilý časovač nabízí řadu nastavení
např. nastavování mezi normálním a inverzním výstupem PWM, generovat přerušení
při dosažení specifické hodnoty časovače apod. @TIMERS.
#figure(
    placement: top,
  caption: [Blokové schéma STM32G031 časovače @STM32G0-REF],
    image("pic/timer_diagram.png"),
)
 
Před spuštěním časovače je potřeba nastavit, jak často má časovač čítat.
Frekvenci časovače nastavuje tzn. prescaler, neboli "předdělička". Prescaler
dělí s konstantou, která je zvolena, frekvenci hodin dané periferie. Pro případ
STM32G0 je to $64$ $"MHz"$.
Frekvence časovače určuje, jak často časovač inkrementuje svou hodnotu za jednu
sekundu @STM32G0-REF.
$ F_("TIMx") = F_("clk")/"Prescaler + 1" $

Velikost čítače časovače, zda je 16bitový nebo 32bitový#footnote[U 16 bitového časovače je maximální perioda 65535, zatímco u 32 bitového časovače je to 4294967295.],
souvisí s jeho tzv. periodou. Perioda určuje hodnotu, při jejímž dosažení se
čítač automaticky resetuje na 0. Tuto hodnotu lze nastavit podle potřeby
vývojáře. V kombinaci s prescalerem lze nastavit konkrétní časový interval,
který je požadován. Časový interval lze vypočítat pomocí #ref(<timer-int>, supplement: [rovnice]).
$ T = (("Prescaler" + 1) × ("Perioda" + 1) )/ F_("clk") $ <timer-int>

=== Knihovna STM HAL
Hardware Abstraction Layer (HAL) od společnosti STMicroelectronics je knihovna určená pro mikrořadiče řady STM32, která slouží jako abstrakční vrstva mezi aplikací a hardwarem zařízení. Jejím hlavním cílem je zjednodušit vývojářům práci s periferiemi, jako jsou GPIO piny, komunikační rozhraní USART, SPI nebo I2C, a to bez nutnosti přímého zápisu do hardwarových registrů procesoru. HAL je součástí širšího ekosystému STM32Cube, který zahrnuje také nástroje pro konfiguraci mikrokontrolérů (např. STM32CubeMX) a generování kódu @STM-CUBE.

Mezi klíčové vlastnosti HAL patří přenositelnost kódu. Protože různé modely MCU STM32 mohou mít odlišné mapování paměti nebo specifické hardwarové vlastnosti, HAL tyto rozdíly abstrahuje. Pokud vývojář potřebuje převést aplikaci na jiný čip z řady STM32, nemusí ručně upravovat adresy registrů a měnit logiku ovládání periferií, ale stačí využít nástroje na konfiguraci jako STM32CubeMX zatímco aplikační kód zůstává nezměněn. Tento přístup šetří čas a snižuje riziko chyb při portování projektů.
@stm32cubemx-arch znázorňuje diagram, který znázorňuje
architekturu HAL @STM-HAL-ARCH.

Součástí HALu je tzv. CMSIS,
což je sada standardizovanách rozhraní, které umožňují konfiguraci periferií,
správu jádra, obsluhu přerušení a další @ARM-CMSIS.
CMSIS je rozdělen do modulárních komponent, kdy vývojář může využít pouze části,
které potřebuje. Např.~CMSIS-CORE, která poskytuje přístup k jádru Cortex-M a
periferiím procesoru, obsahuje definice registrů, přístup k NVIC apod.
@ARM-CMSIS. Hlavní rozdíl mezi CMSIS a HALu#footnote[STMicroelectronics do svého HALu zabaluje i CMSIS od ARM.] STMicroelectronics
je ten, že CMSIS je poskytnuto přímo ARM a slouží pouze na ovládání Cortex M
procesorů zatímco část od STMicroelectronics poskytuje abstrakci periferií.

#figure(
  caption: [STM32CubeMX HAL architektura @HAL-DIAGRAM],
    image("pic/hal-architecture.png"),
) <stm32cubemx-arch>

=== MCU Raspberry Pi Pico
Pro omezenou verzi sondy byl zvolen mikrokontroler Raspberry Pi Pico. Toto je velice populární mikrokontroler, kterým disponují i školy, které nejsou zaměřeny na elektroniku a logické obvody. Studenti těchto škol tak dostanou možnost diagnostikovat své obvody bez nutnosti pořizování speciálních nástrojů. Jediné, co student potřebuje k diagnostice je o jeden Raspberry Pi Pico KIT navíc.

Tento mikrokontroler obsahuje RP2040 s externí flash pamětí o velikosti 2 MiB, s celkem 40 piny @rpi_datashet. RP2040 čip je navržen nadací Raspberry Pi Foundation a je postaven na Dual-core ARM cortex M0+, které dosahují frekvencí až 133 MHz, kterou je možné, stejně jako u STM32G031, měnit pomocí PLL @NI_2024 @rpi_datashet. Tento mikrokontroler je poměrně populární mezi skupinou lidí, která tvoří projekty volnočasově zejména díky ceně, jednoduchého nahrávání programů do mikrokontroleru a komunitní podpoře.

Mikrokontrolér disponuje 26 GPIO piny, které podporují funkce jako pull-up/pull-down rezistory, hardwarová přerušení, PWM či komunikaci přes UART, SPI nebo I2C. Vestavěný 12-bitový ADC umožňuje měření napětí na třech analogových pinech s volitelnou referencí (interní 3,3 V nebo externí). Raspberry Pi Pico je navržené tak, aby bylo možné jej napájet z USB nebo i z externích zdrojů jako baterie.

#figure(
    caption: [Raspberry Pi Pico vývojová deska],
    image("pic/rpi_device.jpg.png")
)
==== Programmable Input/Output (PIO)
Programmable Input/Output blok je unikátní funkcí MCU RP2040, která poskytuje implementaci vlastního rozhraní. RP2040 má tzv. dva PIO bloky, kde každý blok se dá přirovnat k nezávislému malému procesoru, kde můžou běžet instrukce nezávisle na Cortex-M0+ jádře. Tyto bloky umožňují spravovat vstupy a výstupy pinů s velice přesným časováním nezávisle na zátěži CPU. Každý blok má čtyři stavové automaty, které mohou nezávisle na sobě spouštět instrukce, které jsou uloženy ve sdílené instrukční paměti. Každý stavový automat může manipulovat s GPIO a přenášet data do CPU a číst data poslané z CPU. PIO blok má speciální assembler, který obsahuje celkem 9 instrukcí (JMP, WAIT, SET atd.).


== Měření veličin testovaného obvodu <kap-mereni>
=== Měření napětí a logických úrovní
Pro měření napětí, jak již zmiňuje #ref(<adc>, supplement: [kapitola]), je využíván AD převodník. Při měření napětí může docházet k šumu na vstupu kanálu a naměřená hodnota nemusí odpovídat realitě. Pro snížení vlivu šumu je použito tzn. sliding window. Do okna se uloží 32 vzorků měření do dvou bloků tj. 64 vzorků celkem. Každých 320 ms se provede průběžné měření 32 vzorků (vzorkovací frekvence $100$ Hz). Nejstarší blok 32 vzorků je odstraněn a nahrazen novými daty.
#v(10pt)
$ V = (sum_(i=0)^(2^5)(V_"staré i") + sum_(i=0)^(2^5)(V_"nové j")) / 2^6 $
#v(10pt)

Tento přístup kombinuje stabilitu dlouhodobého průměru s reakcí na aktuální změny.
Po aktualizaci okna, které probíhá každých 320 ms, se vypočítá aritmetický průměr z celého okna (64 vzorků), který reprezentuje výsledné napětí#footnote[Jedná se o klouzavý průměr.]. Počet vzorků byl zvolen v mocninách dvojky z důvodu, že dělení může probíhat jako bitový posun, jelikož dělení na MCU je pomalé a paměťově náročné. Měření napětí se vzorkovací frekvencí 100 Hz zajistí, že dojde k potlačení 50 Hz, které se může na vstupu vyskytnout. @fr50 ukazuje, že při 50 Hz rušení odebírá převodník vzorky každých 10 ms (2× za periodu rušivého signálu). Díky odebrání dvou vzorků v protifázích (s odstupem půl periody) a následné integraci sinusového průběhu rušení se jeho vliv v průměru vynuluje, což umožní přesné změření skutečného napětí.
#figure(
    caption: [Odebírání vzorků frekvencí 100 Hz s 50 Hz rušivým signálem],
    image("pic/fr50.png")
)<fr50>

#v(10pt)
#figure(
    caption: [Diagram způsobu sbírání vzorků z ADC],
    placement: none,
    diagram(
        cell-size: (8mm, 10mm),
	    edge-stroke: 1pt,
	    edge-corner-radius: 5pt,
	    mark-scale: 70%,
        blob((0,0), [smazání\ nejstarších\ 32 vzorků], tint: red),
        edge("r", "-|>", label-pos: 0.1),
        blob((1,0), [naměření\ 32 vzorků], tint: yellow),
        edge("r", "-|>", label-pos: 0.1),
        blob((2,0), [Aritmetický \průměr\ z 64 vzorků], tint: green),
        edge((0,0), "d,rr,u", "<|--"),
    )
)
#v(10pt)

Pro zjistění stavu logické úrovně, je nutné vědět nejvyšší možné napětí nízké logické úrovně a nejnižší možné napětí vysoké logické úrovně. Pro CMOS logiku, @cmosil popisuje definici nejvyššího napětí nízké logické úrovně a @cmosih definuje nejnižší napětí logické vysoké úrovně @CMOS. Po změření napětí na kanále pomocí sliding window bude zkontrolováno zda napětí odpovídá logické úrovni nebo je napětí v nedefinované oblasti neboli v oblasti: $V_"ILmax" < V < V_"IHmin"$. Napětí v této oblasti v realném obvodu může vést k nepredikovatelnému chování, proto logická sonda toto napětí detekuje.

#v(10pt)
$ V_"ILmax" = 0.3 times V_"dd" $<cmosil>
$ V_"IHmin" = 0.7 times V_"dd" $<cmosih>



=== Měření odporu
Pro měření neznámého odporu $R_x$ je využit AD převodník (viz #ref(<adc>, supplement: [kapitola])) v kombinaci s napěťovým děličem, jehož schéma je znázorněno na obrázku @divider-img. Rezistory $R_norm$ (normálový rezistor) a $R_x$ (měřený odpor) jsou zapojeny v sérii, čímž tvoří uzavřenou smyčku. Podle Kirchhoffova napěťového zákona platí, že součet úbytků napětí na obou rezistorech je roven napájecímu napětí @DIVIDER_TEXT @Center_2017:
#v(10pt)
$ V_"dd" = V_R_norm + V_R_x $ <ubytek-napet>
#v(10pt)
Pomocí pravidla pro napěťový dělič lze vztah mezi napětími a odpory vyjádřit jako:
#v(10pt)
$ V_R_x = V_"dd" times (R_x)/(R_"norm" + R_x) $<divider-1-rov>
#v(10pt)
Za předpokladu, že $R_norm$ a napájecí napětí $V_"dd"$ jsou známé, a hodnota $V_R_x$ je změřena ADC, lze neznámý odpor $R_x$ vypočítat z upravené #ref(<divider-1-rov>, supplement:[rovnice]) @DIVIDER_TEXT:
#v(10pt)
$ R_x = R_"norm" times (V_R_x)/(V_"dd" - V_R_x) $<divider-2-rov>
#v(10pt)
V praxi probíhá měření neznámého odporu $R_x$ následujícím způsobem: Uživatel sestaví napěťový dělič skládající se z normálového rezistoru $R_norm$ (typicky 10 $k Omega$) a měřeného rezistoru $R_x$. Normálový rezistor je připojen mezi referenční napětí $V_"dd"$ a vstup ADC (kanál 1), zatímco $R_x$ je zapojen mezi tento vstup a zem (viz schéma @divider-img). Sonda následně změří napětí $V_"dd"$ na kanálu 1 ADC a pomocí #ref(<divider-2-rov>, supplement: [rovnice]), vypočítá hodnotu neznámého odporu @DIVIDER_TEXT.

#figure(
    placement: none,
    caption: [Schéma děliče napětí],
    image("pic/divider.png", width: 30%)
)<divider-img>
=== Měření frekvence
==== Metoda hradlování
Pro měření frekvencí v řádu kHz a MHz je využívána metoda hradlování. Tato metoda využívá čítač, který registruje počet náběžných nebo sestupných hran měřené frekvence $N$, za určitý čas~$T_"gate"$. Čas, po který jsou počítány hrany, se nazývá hradlovací čas (angl. gate time). Frekvence $f_"gate"$ touto metodou je vypočítán pomocí #ref(<gate-freq>, supplement: [rovnice]). Délka hradlovacího času může mít vliv na výsledek a proto není vhodné volit jeden čas pro všechny druhy frekvencí. Pokud bude zvolen čas příliš dlouhý, může to zpomalovat měření a také může nastat problém na straně omezenosti hardwaru, kdy při měření vysoké frekvence může dojít k přetečení čítače. V případě příliš krátkého času dojde k nepřesnosti měření a případě nízkých frekvencích nemusí dojít k zachycení správného počtu hran. Proto v případě sondy bude čas volitelný uživatelem.

$ f_"gate" = N/T_"gate" $<gate-freq>

Pro měření metodou hradlování je využit časovač v režimu čítání pulzů, a další časovač pro měření času hradlování, kde tyto dva časovače jsou mezi sebou synchronizovány aby nedocházelo k velké odchylce mezi časem zahájení resp. ukončení činosti čítače a časovače hradlovacího času, případě odchylky může dojít k počítání hran, které nejsou v okně hradlovacího času. Časovač pro měření hradlovacího času je závislý na oscilátoru MCU, kde odchylka frekvence oscilátoru periferie může ovlivnit realný čas měření a ve finále také výslednou frekvenci. Prakticky tato metoda neumožňuje změřit střídu PWM signálu, protože jsou počítány pouze pulzy. 

#figure(
    placement: none,
    caption: [Signál při měření metodou hradlování],
    image("pic/signal-freq-diag.png")
)
==== Reciproční měření frekvence
Reciproční měření frekvence vhodná pro měření nízkých frekvencí $f_"rec"$ (typicky $<$ $1$ KHz). Na rozdíl od hradlování nepočítá hrany za pevný čas, ale měří periodu signálu $T$, ze které frekvenci dopočítá #ref(<rec-freq>, supplement:[vztahem]). Perioda je detekována pomocí náběžné/sestupné hrany, kdy se zahájí měření a měření je ukončeno při další náběžné/sestupné hraně. Během této doby se počítají pulsy interního oscilátoru MCU. 
$ f_"rec" = 1/T $<rec-freq>

Výhoda této metody je možnost výpočtu střídy PWM signálu. V případě, že místo měření celé periody, může být změřen čas od náběžné hrany k sestupné hraně a poté od sestupné k náběžné hraně. Tímto je možno získat šířku pulzu ve vysoké logické úrovni a šířku pulzu v nízké logické úrovni. Pomocí #ref(<strida-freq>, supplement:[rovnice]) je možné dopočítat střídu PWM.

$ D = tau_"high" / (tau_"high" + tau_"low") $<strida-freq>
#figure(
    placement: none,
    caption: [Signál při recipročním měření frekvence],
    image("pic/signal-freq-rec-diag.png")
)


== Analýza komunikačních rozhraní
@rozbor-vyuka zmiňuje testování hardwarových částí obvodu. 
Analýza seriové komunikace je častá nutnost při hledání chyby v implementaci studenta či vývojáře nebo jako otestování funkčnosti součástky. Logická sonda poskytne prostředí pro pasivní poslouchání komunikace, které pomůže vývojáři nalézt chybu v programu nebo studentovi při realizaci školního projektu.
=== Kanál UART<uart>
Universal Asynchronous Reciever Transmiter je rozhraní, kde data jsou odesílána bez společného
hodinového signálu mezi odesílatelem a přijemcem. Místo toho je podstatný
baudrate#footnote[Počet bitů přenesených za sekundu], což určuje počet přenesených bitů za
sekundu. UART podporuje nastavení různých protokolů komunikace jako například
RS-232 a RS-485. UART také umí full duplex komunikaci @USART-REF @WIKI-UART.

Data jsou přenášena v tzv. rámcích, které jsou strukturovány následovně: @WIKI-UART
- *Start bit* - Každý rámec začíná start bitem, který určuje začátek rámce. Bit je
  vždy "0".
- *Slovo dat* - Poté následuje 8 bitů dat#footnote[Lze používat i 7 bitů nebo 9 bitů dat.].
- *Paritní bity* - Paritní bity slouží k detekci chyby v přenosu. Parita nám
  dokáže pouze detekovat chybu rámce pouze v případech, kdy nevznikne chyb více#footnote[Chyba z dat 1101 na 1000 nelze detekovat, protože lichá parita má paritní bit v
    obou případech 0.].
- *Stop bit* - Stop bit#footnote[Stop bitů může být i několik] signalizuje konec
  přenosu rámce. Obvykle logická "1".

Pokud rozhraní neodesílá žádné bity, na vodičích se nachází vysoká úroveň. Této vlastnosti bude využito později v návrhu logické sondy.

V logické sondě je UART využíván, ke komunikaci s PC a také logická sonda umí toto rozhraní pasivně sledovat i aktivně odesílat testovací sekvence @uart-signal-picture ukazuje způsob zpracování signálů @UART-SIGNAL-PICTURE. Testování tohoto rozhraní je potřeba pokud student či vývojář potřebuje najít chybu např. při implementaci seriové komunikace mezi dvěma mikrokontrolery, kde se právě často využívá UART.
#figure(
    placement: none,
  caption: [Způsob zpracování signálu UART bez parity], image("pic/UART-signal.png"),
)<uart-signal-picture>

=== I2C Bus
*Inter-Integrated Circuit* je seriová komunikační sběrnice, který byl vytvořen Philips Semiconductor jako nízkorychlostní sběrnice pro propojení zařízení jako např. mikrokonkrolery a procesory se senzory, periferiemi apod. Od roku 2006 implementace protokolu nevyžaduje licenci a proto se začal široce používat např. v IOT. Výhoda sběrnice je, že pro komunikaci potřebuje pouze dva vodiče, na které je možné připojit až 128 zařízení najednou, jelikož využívá systém adres @I2C_TI.

SCL vodič, slouží jako hodinový signál a SDA vodič slouží jako datový vodič. Protokol rozlišuje zařízení typu master a slave. Master řídí hodinový signál a protože je I2C obousměrný half duplex protokol, tak master zahajuje a zastavuje komunikaci aby nedocházelo ke konfliktům. Oba vodiče mají otevřený kolektor, z důvodu, že je na lince připojeno více zařízení a vodiče jsou pull up rezistorem přivedeny na společný zdroj napětí, což znamená, v klidovém režimu, jsou na vodičích vysoké logické úrovně @I2C_TI.
#v(10pt)
#grid(
    columns: 2,
    gutter: 10pt,
    figure(
        image("pic/i2c_start_stop.png"),
        caption: [Zahájení a ukončení komunikace v I2C @I2C_TI],
    ), 
    [#figure(
        image("pic/i2c_zeros_ones.png", height: 113pt),
        caption: [Logická jednička a logická nula v~I2C @I2C_TI],
    )<i2c-zeros-ones>]
)
#v(10pt)

Pro zahájení komunikace, master is zarezervuje sběrnici posláním `I2C START`. Nejprve master přivede vodič SDA do nízké logické úrovně a následně tam přivede i SCL. Tato sekvence indikuje, že se master zahajuje vysílání a všechny zařízení poslouchají. Pro ukončení je nejprve uvolněn SCL a až poté SDA. Tím je signalizováno, že komunikace je ukončena a jiný master může komunikovat.

@i2c-zeros-ones ukazuje způsob odesílání jednotlivých bitů. Pro odeslání logické jedničky SDA uvolní vodič, aby byla přivedena přes pull up rezistor vysoká logická úroveň. Pro logickou nulu, vysíláč stáhne vodič do nízké úrovně. Přijimač zaznamená bit v momentě, kdy SCL zapulsuje.

Protokol rozděluje bity do rámců. Rámec má vždy 8 bitů. Nejprve pošle adresový rámec, který identifikuje, který slave má reagovat. Součástí rámce je také read-write bit.  Pokud slave přečte adresový rámec a daná adresa mu nepatří, ignoruje komunikaci.  V opačném případě odpoví `ACK` bitem, kdy nízká úroveň znamená potvrzení. Vysoká úroveň nastane, když slave nezareaguje a vodič zůstane v klidu, tzn. vysoká úroveň.

Po identifikaci se zahájí odesílání datových rámců, které se skládají z 8 bitů a jsou zakončeny `ACK`. Pokud byl read-write bit nastaven na read, master většinou zašle adresu z které chce číst a slave pošle obsah paměti. Při write, master zašle adresu na kterou chce zapisovat a následně data, které chce zapsat @I2C_TI.
#figure(
    image("pic/i2c_frames.png"),
    caption: [Rámce I2C @I2C_TI]
)

=== Rozhraní SPI
*Serial peripherial interface* je jeden z nejvíce využívaných rozhraní používaný mezi mikrokontrolery a periferiemi jako např. AD převodníky, SRAM, EEPROM apod. Rozhraní nemá definované jaké napětí se používají a ani velikosti rámců. Typicky se používá 8 bitů. Oproti UART a I2C vyniká rychlostí komunikace, která je v řádu MHz.

SPI má vždy jedno master zařízení a i několik podřízených slave zařízení. SPI je synchronní full duplex rozhraní, které má celkem 4 vodiče#footnote[4 vodiče má v případě jednoho slave zařízení. S každým dalším slave zařízením musí být připojen další `SS`.]. `SCLK`, což je hodinový signál, který určuje synchronizaci dat, `MOSI`, neboli vodič, kde probíhá komunikace od masteru ke slave, `MISO`, kde probíhá komunikace od slave k masteru a poté `SS`#footnote[Někdy také `CS` jako chip select.], neboli slave select. Ten určuje, se kterým slave zařízením probíhá komunikace, každé zařízení má vlastní `SS` pin @ANALOG-SPI.
@pic-spi-diagram-single ukazuje způsob zapojení vodičů v případě jednoho slave zařízení. Pro zahájení komunikace nastaví logicky nízkou úroveň na `SS`#footnote[Jelikož se spíná vodič do log. 1, tak má značka `SS` nad sebou negaci.]. Master zahájí generování hodinového signálu, podle kterého se synchronizuje komunikace. Jelikož je komunikace full duplex, komunikace začne probíhat mezi master a slave oběma směry tzn. na vodičích `MISO` a `MOSI`. Po dokončení komunikace master ukončí vysílání hodinového signálu a nastaví `SS` na vysokou logickou úroveň. @pic-spi-diagram-multi znázorňuje připojení více slave zařízení. V tomto případě master využívá více `SS` a podle přivedení nízké logické úrovně určuje směr komunikace @ANALOG-SPI.

#v(10pt)
#grid(
    columns: 2,
    [
        #figure(
            caption:[Diagram SPI komunikace s jedním slave zařízením],
            image("pic/spi_single_diagram.png")
        )<pic-spi-diagram-single>],
    [
        #figure(
            caption:[Diagram SPI komunikace s více slave zařízeními],
            image("pic/spi_multi_diagram.png")
        )<pic-spi-diagram-multi>
    ]
)
#v(10pt)


Zjištění logické 0 a 1 vychází z přečtení logické úrovně v momentě vzestupné nebo sestupné hraně hodinového signálu. Vztah mezi hodinovým signálem a daty tzn. `CPOL` bitem a `CPHA` bitem. `CPOL` bit určuje, jakou logickou úroveň má klidový stav hodinového signálu. Při log. 0 je klidová úroveň nízká a hodinový signál započne náběhovou hranou, při log. 1 naopak. `CPHA` určuje, jaká hrana má určovat logickou úroveń signálu, při log. 0 je čtena hodnota při první hraně signálu, při log. 1 je čtena hodnota při druhé hraně hodinového signálu#footnote[Pokud bude `CPOL` bit = 0 a `CPHA` = 0, tak signál bude čten při náběžné hraně, při `CPOL` = 0 a `CPHA` = 1 bude čtena při sestupné hraně.].
#figure(
    placement: none,
    caption: [Časový diagram SPI zobrazující úroveň a posun hodinového signálu @WIKI-SPI-DIAGRAM],
    image("pic/spi-cpol.png", width: 70%)
)



=== RGB LED Neopixel s digitálním rozhraním<kap-neopixel-ter>
Neopixel je název pro kategorii adresovatelných RGB LED. Dioda má celkem 4 vodiče: ground, Vcc, DIn a DOut. LED má vlastní řídící obvod, který ovládá barvy diody na základě signálu z vodiče DIn. Výhoda LED je možnost připojit diody do serie, a jedním vodičem ovládat všechny LED v sérii @NEOPIXEL-REF. @label-neopixel znázorňuje zapojení více LED do série a~schopnost ovládání jedním vodičem.

Data do LED se zasílají ve formě 24 bitů, kdy každých 8 bitů reprezentuje jednu barevnou složku. Parametry pořadí složek, časování apod. se může lišit v závislosti na konkrétním verzi a provedení LED. V této práci je vycházeno z WS2812D. První bit složky je, v případě WS2812, MSB#footnote[Most significant bit]. 
#figure(
  image("pic/NEOPIXEL_SCHEME_SERIE.png", width: 80%),
  caption: [Způsob zapojení RGB LED do série @NEOPIXEL-REF],
) <label-neopixel>
    #figure(
    caption: [Diagram posílání dat pro zapojené WS2812D v sérii @NEOPIXEL-REF],
    image("pic/neopixel_signal.png", width: 90%))
#v(5pt)
#figure(
  placement: none,
  caption: [Pořadí bitů pro nastavení barvy v WS2812 @NEOPIXEL-REF],
  text(size: 9pt)[
    #table(
      columns: 24,
      align: center + horizon,
      table.header(
        "R7", "R6", "R5", "R4", "R3", "R2", "R1", "R0",
        "G7", "G6", "G5", "G4", "G3", "G2", "G1", "G0",
        "B7", "B6", "B5", "B4", "B3", "B2", "B1", "B0"
      ),
    )
  ]
)<neopixel-bits>
#v(5pt)

Neopixel nepracuje na sběrnici s časovým signálem, proto je nutné rozpoznávat logickou jedničku a nulu jiným způsobem. Na pin DIn je přivedena vysoká úroveň na~určitou dobu, poté je na určitou dobu přivedena nízká úroveň. Kombinace těchto časů dává řídícímu obvodu v LED možnost rozpoznat, jaký bit byl poslán diodě. Pro ovládání `n`~LED, na DIn první LED je zasláno $n × 24$ bitů. Dioda zpracuje prvních 24 bitů, a na Dout odešle $(n-1)×24$ bitů. Tento proces se opakuje pro každou LED v sérii a tím je dosaženo rozsvícení všech diod na požadovanou barvu. Aby řídící obvod rozpoznal, které data má poslat dále a která jsou už nová iterace barev pro LED, je nutné dodržet tzn.~RESET time, kdy po uplynutí tohoto času, řídící obvod, už neposílá data dále, ale zpracuje je @neopixel_bit_time ukazuje časování pro WS2812D. Po testování odesílání dat do RGB LED bylo zjištěno, pro LED jsou stěžejní časy vysokých úrovní, nicméně časy nízkých úrovní jsou více benevolentní a bylo například zjištěno, že nízká úroveň u log. 1 je možné zkrátit i na čas $330$ ns.
#v(10pt)
#figure(
    placement: none,
    caption: [Časování logických úrovní pro zaslání bitů WS2812D @NEOPIXEL-REF],
    table(
        columns: 3, 
        align: center,
        table.header("Bit", "Typ času", "t[ns]"),
        [0], [vysoká úroveň napětí], [$220~380$],
        [0], [nízká úroveň napětí], [$580~1000$],
        [1], [vysoká úroveň napětí], [$580~1000$],
        [1], [nízká úroveň napětí], [$580~1000$],
        [RESET], [nízká úroveň napětí], [$>280$ $000$],
    )
)<neopixel_bit_time>


= HW návrh logické sondy STM32 <kap-hw>
 Návrhy obsahují, co nejméně komponent, aby student byl schopný zařízení jednoduše sestavit. Tzn. například pull up nebo pull down rezistory jsou řešeny interně na pinu. Logická sonda musí být ideálně co nejvíce kompatibilní mezi oběma pouzdry, tak aby byla zaručena přenositelnost a pravidla pro sestavení byla co nejvíce podobná.
== Sdílené vlastnosti mezi návrhy pouzder<komp>
Sonda je napájena skrze UART/USB převodník. Jelikož USB pracuje s napětím $5$ V ale STM32G030 vyžaduje napájecí napětí  $1.7 ~ 3.6$~V @STM32G0-REF je nutné napětí snížit. Proto byl použit linearní stabilizátor *HT7533*, který stabilizuje napětí na $3.3$~$plus.minus$~$0.1$~V. Ke vstupu je připojen kondenzátor `C1` k potlačení šumu o velikosti $10$ $mu$F. K výstupu je připojen keramický kondenzátor#footnote[Keramický s důvodu, že LDO požadují nízké ESR] `C2` k zajištění stability výstupu o velikosti také $10$ $mu$F @HT7533.
#v(10pt)
#figure(
    placement: none,
    caption: [Zapojení regulátoru pro napájení STM32G030 @fischer-regulator],
    image("pic/regulator.png", width: 90%)
)<hw-regulator>
#v(10pt)

Návrh zohledňuje implementaci lokálního režimu. Pro tuto implementaci je na pin `PA13` zapojeno tlačítko pro interakci s uživatelem vůči zemi s interním pull up rezistorem na pinu. Připojení vůči zemi minimalizuje riziko zkratu chybným zapojení uživatelem.

Dále je připojena WS2812 RGB LED na `PB6`. Tento pin byl zvolen z důvodu přítomnosti kanálu časovače, který je využit pro posílání dat skrze PWM do LED. WS2812 dle datasheetu vyžaduje napětí $3.7 ~ 5.3$ V @NEOPIXEL-REF. Pokud by WS2812 byla napájena $5$ V z USB převodníku, došlo by k problému s CMOS logikou, kdy vstupní vysoká logická úroveň je definována jako $0.7 times V_"dd"$, což se rovná $3.5$ V a STM32 pin při vysoké úrovni má $V_"dd"$, což je $~3.3$ V @CMOS @STM32G0-REF. Z toho důvodu je navzdory datasheetu LED připojena na napětí $V_"dd"$ mikrokontroleru. Toto zapojení bylo otestováno a je plně funkční. Problém se kterým je možné se setkat je nesprávné svícení modré barvy z důvodu vysokého prahového napětí. Mezi katodu a anodu LED je umíštěn blokovací kondenzátor o velikost $100$ nF.

Obě pouzdra využívají pro komunikaci s PC periferii USART1. STM32 poskytuje možnost remapování pinů. Pro zjednodušení zapojení jsou piny `PA12` a `PA11` přemapované na `PA10` a `PA9`. Tyto piny jsou použity jako Tx a Rx piny UART komunikace. Pro zajištění funkce lokálního režimu je na Rx pin přiveden pull down rezistor o velikosti $10$ K$Omega$.

== STM32G030 v pouzdře SOP8
@sop8-hw#footnote[Schéma zapojení bylo zrealizováno pomocí nástroje _Autodesk Eagle_ @EAGLE_SW. Komponenta Neopixel RGB LED byla použita jako externí knihovna @NEOPIXEL-SCHEMA-LIB.] ukazuje zapojení STM32G030 v pouzdře SOP8. Toto pouzdro po zapojení napájení, rozhraní UART má k dispozici pouze 4 piny. Po zapojení potřebných komponent pro lokální režim, které zmiňuje @komp, zůstávají piny 2. Z tohoto důvodu, na pouzdro SOP8, jsou implementován pouze lokální režim a základní funkce terminálového režimu, jako měření napětí, frekvence a vysílání pulzů.
#v(10pt)
#figure(
    placement: none,
    caption: [STM32G030Jx SO8N Pinout @STM32G030x6-tsop],
    image("pic/sop8_pinout.png", width: 80%),
)<sop8-pinout>
#v(10pt)

Jelikož je pouzdro malé, tak se na jednom fyzickém pinu nachází více periferií. @sop8-pinout ukazuje, že na pinu 4, kde se nachází `PA0`, má připojený i `NRST`. `NRST` požaduje aby pin byl neustále ve vysoké logické úrovni, což pro potřebu logické sondy je nepraktické protože takto není možné využít `PA0`. Funkce nresetu lze vypnout skrze tzv. *optional bits*. Kde na pozici `NRST_MODE` je potřeba nastavit `2`, aby NRST byl ignorován a `PA0` bylo použitelné. Pomocí nastavení bude zajištěno, že `NRST` bude ignorován po dobu běhu programu, nicméně při bootu je nežádoucí, aby byl přiveden na logicky nízkou úroveň, protože stále MCU v této fázi ignoruje nastavení optional bitu.
#v(10pt)
#figure(
    placement: none,
    caption: [Paměťový prostor Flash option bits @STM32G0-REF],
    image("pic/option_bits.png"),
)<optional-bits>
#v(10pt)

Další problém představuje pin 8, který obsahuje `PA14-BOOT0`. Při startu MCU bootloader zkontroluje bit *FLASH_ACR*, který určuje jestli je FLASH paměť prázdná. Pokud ano, MCU zapne a začne poslouchat periferie kvůli případnému stáhnutí firmwaru do FLASH paměti. Pokud FLASH prázdná není, program uložený v paměti se spustí. Pokud je na `PA14-BOOT0` ve vysoké logické úrovni, MCU se chová stejně, jako by paměť byla prázdná @STM32G0-REF. Standartně se mikrokontroler nahrává a debuguje pomocí tzn. SWD#footnote[Serial Wire Debug slouží pro jednoduší vývoj na mikrokontrolerech, je možné číst FLASH, RAM, nahrávat program, nastavovat option bity apod.], nicméně při této konfiguraci je to nepraktické, protože, by to znamenalo připojit ST-LINK k mikrokontroleru, nahrát, odpojit a poté až udělat zapojení, které ilustruje @sop8-hw. Pro jednoduchost se firmware nahraje pomocí UART. V tomto případě je ale potřeba řídit, zda má být nahráván firmware nebo spuštěn program. Optional bit `nBOOT_SEL` určuje, zda má být toto řízeno pomocí bitů `nBOOT0` a `nBOOT1` nebo pomocí úrovně `PA14-BOOT0`. V případě sondy, je potřeba druhá možnost, takže je nutné nastavit bit `nBOOT_SEL` na `0`.
#figure(
    placement: none,
    caption: [Schéma zapojení STM32G030 v pouzdře SOP8],
    image("pic/sop8_hw.png", width: 80%),
)<sop8-hw>
První z pinů k užívání je pin `PB7`. Tento pin slouží jako kanál AD převodníku pro měření napětí a pro měření odporu, pin je také využit pro hodinový signál pro posuvný registr.
Na pinu `PA0` se nachází AD převodníkový kanál. Pin také disponuje kanály TIM2 časovače. Pin je použit jako druhý kanál AD převodníku pro měření napětí, pro posuvný registr je pin využíván pro posouvání dat do posuvného registru, měření frekvence, odchytávání Neopixel dat, detekce pulsů a generování frekvence. Pin `PB6` je použit pro odesílání dat do testované RGB LED.
== STM32G030 v pouzdře TSSOP20<tssop20>
Pouzdro TSSOP20 nabízí oproti SOP8 výhodu většího počet pinů a tím pádem i jednodušší implementaci pokročilých funkcí. Pouzdro má celkem 20 pinů, což má za následek, že např. může být pin `NRST` (pin 6) oddělen od `PA0` a má tak vlastní pin. Z tohoto důvodu při flashování MCU není potřeba myslet na nastavení optional bits pro `NRST` a může zůstat v základním nastavení. Nicméně pin `PA14-BOOT0` musí být nastaven stejným způsobem jako u SOP8, tzn. optional bit `nBOOT_SEL` je nutné nastavit na `0` aby bylo možné při startu MCU určit, zda má být nabootován program ve FLASH paměti, nebo má poslouchat periferie pro nahrání programu. 

Pro zjednodušení sestavení sondy, je HW TSSOP20 návrh co nejvíce podobný návrhu SOP8.  Piny `PA11` a `PA12` jsou přemapovány na `PA9` a `PA10`. Na pin `PA10` je připojen rezistor o velikosti 10 $K Omega$ vůči zemi pro detekci komunikace s PC. Ze stejného důvodu byl zachován pin `PB6` jako výstup pro WS2812D a `PA13` pro tlačítko pro lokální režim. @tssop20-hw ukazuje schéma zapojení s pouzdrem TSSOP20. Rozmístění pokročilých funkcí vychází z charakteristik jednotlivých pinů. Pin 1 (`PB7`) je využit stejně jako v pouzdře SOP8 jako první kanál ADC. Na pinu 1 (`PB8`) a 2 (`PB9`) se nachází I2C periferie a proto jsou využity pro sledování komunikace I2C sběrnice. Pin 7 (`PA0`) je k měření frekvence a napětí. Piny 9 (`PA2`) a 10 (`PA3`) mají USART periferii a proto jsou vhodní kandidáti na sledování UART komunikace. Piny 12 (`PA5`), 13 (`PA6`), 14 (`PA7`) a 15 (`PB0`) mají SPI rozhraní a proto jsou použity pro sledování SPI komunikace. $V_"dd"$ je připojeno na výstup linearního stabilizátoru z #ref(<hw-regulator>, supplement: [obrázku]), který má na výstopu $3.3$ $V$.

#figure(
    caption: [STM32G030Jx TSSOP20 Pinout @STM32G030x6-tsop],
    image("pic/tssop20_pinout.png", width: 100%)
)<tssop20-pinout>

#figure(
    caption: [Schéma zapojení STM32G030 v pouzdře TSSOP20],
    image("pic/tssop20_hw.png", width: 80%)
)<tssop20-hw>

= #set text(size: 23pt); Návrh terminálového režimu STM32
== Princip oblužní smyčky
Terminálový režim využívá rozhraní UART, pro sériovou komunikaci s PC. Způsob vstoupení do terminálového režimu rozebírá #ref(<kap-log-rezim>, supplement: [kapitola]). Základ terminálového režimu běží v nekonečné smyčce, která je na konci oddělena čekáním #footnote[Toto čekání se mění na základě zvolené funkce.]. Smyčka slouží jako obsluha akcí, které jsou vyvolány, jak uživatelem prostřednictví TUI, tak periferiemi, které momentálně běží. Obsluha při každé iteraci provede jednotlivé úkony, pokud příznaky v globální struktuře (@code-global_vars_t) jsou nastaveny. Příznaky jsou běžně nastavovány skrze přerušení, například vyvolané uživatelem skrze odeslání symbolu seriovou komunikací. Obsluha v každé iteraci zkontroluje, zda příznak `need_frontend_update` vyžaduje vykreslit grafiku TUI (@kap-tui), zda příznak `need_perif_update` vyžaduje změnit periferii (@kap-perif), poté vykreslí data, které periferie získala a nakonec čeká na další smyčku. Sonda vykresluje data na základě `device_state` promněné, která určuje, jakou funkci uživatel momentálně používá.
#v(10pt)
#figure(
caption:[Diagram smyčky terminálového módu],
    placement: none,
    diagram(
        cell-size: (8mm, 10mm),
	    edge-stroke: 1pt,
	    edge-corner-radius: 5pt,
	    mark-scale: 70%,
        blob((1,0.3), [start], tint: yellow, extrude: (0,3)),
        edge((1,1),"-|>"),
        blob((0,1), [Vyžádáno\ nové nastavení\ periferií?], shape: shapes.hexagon, tint: red),
	    edge("r", "=>", [Ano]),
    	edge("d", "=>", [Ne]),
        blob((1,1), [Nastavit\ periferie], tint: yellow),
    	edge("d,l", "-|>", []),
        blob((0,2), [Vyžádána\ aktualizace\ rozhraní?], shape: shapes.hexagon, tint: red),
    	edge("l", "=>", [Ano]),
    	edge("d", "=>", [Ne]),
        blob((-1,2), [Vykreslit\ aktuální\ grafiku\ celé stránky], tint: yellow),
    	edge("d,r", "-|>", []),
    	blob((0,3), [Vykreslit\ naměřené\ hodnoty], tint: yellow),
    	edge("d", "-|>", []),
    	blob((0,4), [Čekej], corner-radius: 2pt, tint:orange, extrude: (2,4)),
        edge((0,4), (-1.7,4), "uuu,rr", "--|>"),
    )
)<diagram-terminal-mod>

Metoda periodické obsluhy nastavování periferií a vykreslování TUI, oproti okamžité reakci přímo v přerušení má výhodu v tom, že nemůže dojít k překrytí činností mezi hlavní smyčkou a přerušeními. Např. pokud bude stránka periodicky vykreslována, a stisk tlačítka by vyvolal přerušení k překreslení programu, může se přerušit smyčka v momentě, kdy už k překreslení dochází. V tomto případě poté dojde k rozbití obrazu vykresleného na terminál. Obdobná věc hrozí při vypínání a zapínání periferií. Kdy průběh deinicializace periferie přerušen a nastane inicializace, může dojít k nepredikovatelnému chování. Metodou obsluhy jsou definovány posloupnosti úkonů, které se nemohou překrývat.

== TUI řešení <kap-tui>
@rozbor-vyuka zmiňuje důraz na jednoduchou přístupnost ve výuce, což zahrnuje i jednoduché zobrazení informací, které uživatel potřebuje. Aby zprovoznění sondy bylo co nejjednodušší, nebyla zvolena cesta ovládání skrze specialní PC aplikaci nebo specialní ovladač, ale byla zvolena cesta ovládání sondy skrze libovolnou terminálovou aplikaci podporující ANSI escape sekvence #footnote[Například program PuTTY...]. ANSI escape sekvence zajistí možnost pseudo-grafického prostředí skrze terminál. Ke generaci uživatelského rozhraní bude docházet na straně mikrokontroleru a posíláno UART periferií do PC. Tento způsob navíc zajistí nezávislost na operačním systému a je možné komunikovat se sondou na jakémkoliv populárním operačním systému.

=== Ansi sekvence
ANSI escape kódy představují standardizovanou sadu řídicích sekvencí pro manipulaci s textovým rozhraním v terminálech podporujících ANSI/X3.64 standard. Tyto kódy umožňují dynamickou úpravu vizuálních vlastností textu (barva, styl), pozicování kurzoru a další efekty, čímž tvoří základ pro tvorbu pokročilých terminálových aplikací @Grainger.
==== Syntaxe
Základní syntaxe escape sekvencí pro formátování textu je:
```bash
\033[<parametry><akce>
```
- `\033` (ASCII 27 v osmičkové soustavě) označuje začátek escape sekvence#footnote[Existují také `\033` N nebo `\033` \\ apod.ale tyto se téměř nepoužívají.]

- `[` je úvodní znak pro řídicí sekvence
- `<parametry>` jsou číselné kódy oddělené středníky
- `<akce>` je písmeno specifikující typ operace
==== Formátování textu
Pro změnu barvy a obecně textu je použito písmeno `m` jako akce. Nejčastější parametry s popisem vypisuje @ansi-text-codes. 
#figure(
    placement: none,
    caption: [Tabulka akcí ANSI sekvencí],
table(
  columns: 3,
  align: center,
  inset: 5pt,
  stroke: (bottom: 0.5pt + black),
  [*Kód*],          [*Typ*],              [*Popis*],
  // Barvy textu
  [30–37],          [Text ∙ Základní],    [Černá, Červená, Zelená, Žlutá, Modrá, Purpurová, Tyrkysová, Bílá],
  [90–97],          [Text ∙ Světlé],      [Světlé varianty základních barev],
  [40–47],          [Pozadí ∙ Základní],  [Černé, Červené, Zelené... pozadí],
  [100–107],        [Pozadí ∙ Světlé],    [Světlé varianty pozadí],
  // Textové efekty
  [0],              [Efekt],              [Reset všech stylů],
  [1],              [Efekt],              [Tučný text],
  [4],              [Efekt],              [Podtržení],
  [7],              [Efekt],              [Inverzní barvy]
)
)<ansi-text-codes>
==== Manipulace s kurzorem
Sekvence také lze použít pro pohyb kurzoru, což je užitečné pro vizuál aplikace. Pro pohyb kurzoru na konkrétní pozici zajišťuje písmeno `H` a pro pohyb o relativní počet symbolů slouží písmena `A` jako nahoru, `B` jako dolu, `C` jako doprava a `D` jako doleva na pozici akce @GITHUB-ANSI.
#v(5pt)
```bash
\033[<row>;<col>H // Pohyb na konkrétní pozici
\033[<posun><směr> // Posune kurzor o danou pozici
\033[10;15H // Posune kurzor na pozici 10. řádku a 15 sloupce
\033[10B // Posune kurzor o 10 řádků dolů
```
#v(5pt)
==== Mazání obsahu
ANSI escape kódy umožňují kromě formátování textu také dynamické mazání obsahu obrazovky nebo řádků, což je klíčové pro aktualizaci TUI. Tyto sekvence se využívají např. pro překreslování statických prvků nebo odstranění přebytečného textu @GITHUB-ANSI.
.
```bash
    \033[2J // Smazání celého displeje
    \033[0K // Smazání textu od pozice kurzoru do konce řádku
    \033[1K // Smazání textu od pozice kurzoru do začátku řádku
    \033[2K // Smazání celého řádku
    \033[2KProgress: 75% // Smazání řádku a vypsání (Progress: 75%)
```
=== Nastavení periferie pro zobrazování TUI
Pro komunikaci s PC je použito rozhraní `USART1`, jehož výchozí piny `PA9` (TX) a~`PA10` (RX) byly remapovány na alternativní piny `PA11` a `PA12`. Periferii je možné inicializovat pomocí programu STM32CubeMX, který po nastavení parametrů vygeneruje příslušné inicializační a deinicializační funkce. Pro komunikaci byl zvolen baudrate `115200` a 8~bitové slovo s jedním stop bitem bez parity, což například u programu PuTTY je základní nastavení, takže není nutné aby uživatel něco dalšího nastavoval.

#figure(
    caption: [Inicializace UART periferie],
    supplement:[Úryvek kódu],
    ```C
static void MX_USART1_UART_Init(void) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B; // velikost dat
    huart1.Init.StopBits = UART_STOPBITS_1; // počet stop bitů
    huart1.Init.Parity = UART_PARITY_NONE; // bez parity
    huart1.Init.Mode = UART_MODE_TX_RX; // Zapnut full duplex
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
}
```
)<code-uart1-init>

=== Vykreslování stránek
Stránky jsou grafická reprezentace zvolené funkce. Tyto stránky vykreslují ovládací prvky, data získané periferiemi či varovné zprávy. Každá stránka je vykreslována skrze USART1 periferii skrze jednoduchou funkci (@code-send_str), která pošle string skrze periferii o dané velikosti. Na této funkci poté staví další funkce, které dokážou sestavovat větší celky. Funkce z #ref(<code-set_cursor>, supplement: [úryvku kódu]) nastavuje, dle pravidel zmíněných výše, kurzor na příslušné souřadnice. Ve funkci se také nachází kontrola, zda se kurzor nachází v rámci rozměrů stránky, které jsou fixně nastavené na $80 times 24$. Tyto rozměry jsou v terminálové aplikaci ohraničeny ASCII symboly. Pro vykreslení textu je využita funkce z #ref(<code-uart_text>, supplement: [úryvku kódu]), kde k danému textovému řetězci jsou před odesláním přidány ANSI sekvence pro podbarvení pozadí, textu a nebo ztučnění symbolů. Aplikace těchto všech funkcí lze vidět na //#ref(<tui-menu>, supplement:[obrázku]), kde je vygenerována hlavní stránka sondy. Stránky jsou vykreslovány dvěma způsoby.

#figure(
    caption: [Způsob odeslání stringu UART periferií],
    supplement:[Úryvek kódu],
    ```C
void ansi_send_string(const char* str) {
    HAL_UART_Transmit(USART1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}
    ```
)<code-send_str>

#figure(
    caption: [Nastavení pozice kurzoru pomocí ANSI escape sekvencí],
    supplement:[Úryvek kódu],
    ```C
void ansi_set_cursor(const uint8_t row, const uint8_t col) {
    if (row > TERMINAL_HEIGHT || col > TERMINAL_WIDTH) {
        Error_Handler();
        return;
    }

    char result[CURSOR_BUFF_SIZE];
    size_t ret = snprintf(result, CURSOR_BUFF_SIZE, "\033[%u;%uH", row, col);

    if (ret >= sizeof(result)) {
        Error_Handler();
    }
    ansi_send_string(result);
}
```
)<code-set_cursor>

Každá stránka má tzv. statickou část, která se po celou dobu nemění. Statická část je vždy vykreslena na začátku vstupu stránky a poté je vždy vykreslena oblužní smyčkou v momentě, kdy příznak `need_frontend_update` je nastaven. V případě nastavení příznaku obslužní smyčka odešle ANSI sekvenci `\033[2J`, která smaže celou stránku a poté vykreslí stránku odpovídající aktualně zvolené funkci. Příznak lze taky manuálně nastavit odesláním symbolu `R`, který je užitečný v případě, kdy se například vlivem špatného kontaktu vodiče mohou generovat náhodné symboly. Pokud by statická část vykreslovala periodicky, může dojít ke zbytečnému odesílání velkého množství dat skrze UART a to může spomalovat vykreslování. Také může dojít k rychlému blikání kurzoru v terminálové aplikaci, což je nežádoucí.

Tzv. dynamická část stránky se vykresluje každý cyklus oblužní smyčky. Do dynamické části spadá vypisování varovných zpráv, naměřených hodnot a výstupů z periferií. Hodnoty jsou vždy zobrazeny s definovaným počtem číslic. Např. napětí ve voltech je formátováno jako `printf("%4d")`, což zobrazí 4 číslice čísla a pokud má číslo méně než 4 číslice, je jsou pozice nahrazeny mezerou. Při generování upozornění je řádek, na kterém se text vypisuje, smazán ANSI sekvencí `\033[2K` a v případě potřeby je zobrazen nový text. #ref(<tui-ohm-static>, supplement: [Na obrázku]) je znázorněno na příkladu stránky pro měření odporu, že je ASCII ART zapojení, popisky a ohraničení vykresleno staticky a hodnoty, které jsou naměřeny jsou vykreslovány dynamicky.

#figure(
    caption: [Ukázka vykreslování statické a dynamické části stránky],
    image("pic/tui_ohm_static.png")
)<tui-ohm-static>

=== Ovládání uživatelského rozhraní
Ovládat sondu lze pomocí symbolů, odesílané na rozhraní UART skrze terminálovou aplikaci. Na straně MCU jsou symboly přijímány na periferii UART, která při obdržení symbolu vyvolá přerušení. Pro implementaci zpracování symbolu je použit callback `HAL_UART_RxCpltCallback`, který je zavolán při vyvolání přerušení. Callback přečte symbol, který byl přijat a zkontroluje, zda to není symbol, který je obecný pro všechny stránky#footnote[Obecně je to symbol `R`, který slouží znovu vykreslení.]. V případě jiných symbolů je nahlédnuto do globální promněné `current_page` (viz. @code-global_vars_t), která uchovává informaci, na které stránce se momentálně uživatel nachází a v závislosti na tom, je zvolena funkce pro provedení akce na základě přijatého symbolu. Po provedení příslušné akce je opět zapnuto přerušení pro přijetí znaku na UART periferii (viz. @code-UART-get). Způsob přepínání ovládání v závislosti na stránce ukazuje #ref(<code-uart-parse-page>, supplement: [úryvek kódu]). Způsob převedení symbolu na akci na dané stránce ukazuje příklad ovládání hlavního menu v #ref(<code-uart-parse-menu>, supplement: [úryvku]). V tomto úryvku lze vidět, že pomocí přepínače je ovládání nezávislé na tom, zda uživatel posílá velká nebo malá písmena. Samotný callback nevykresluje stránku, ale pouze nastavuje příznak `need_frontend_update`, aby v dalším obslužním cyklu byla stránka vykreslena.
#figure(
    caption: [Způsob odeslání stringu UART periferií],
    supplement:[Úryvek kódu],
    ```C
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == UART_INST) {
        if (global_var.received_char == 'r' ||
            global_var.received_char == 'R') {  // reload
            ansi_clear_terminal();
            ansi_render_current_page();
            global_var.booted = true;
        } else {
            get_current_control();
        }

        HAL_UART_Receive_IT(&UART, &global_var.received_char, 1);
    }
}
```
)<code-UART-get>

=== Struktura TUI
Po připojení sondy je uživatel přivítán stránkou hlavního menu (#ref(<tui-menu>, supplement: [obrázek])). Tato stránka je hlavní rozcestník mezi funkcemi. Po stisknutí příslušné klávesy u funkce je uživatel přesměrován na konkrétní stránku s funkcí. Hlavní menu je rozcestník pro tzv. základní módy, které jsou k dispozici, jak na SOP8 tak na TSSOP20. Při stisku písmene `A`, se uživatel v případě pouzdra TSSOP20 dostane do pokročilých funkcí, kde se nachází monitorování periferií. Univerzálně platí, že symbolem `Q` se uživatel dostane vždy to tohoto menu, z kterého poté může zvolit jinou funkci.

#grid(
    columns: 2,
    [#figure(
        caption: [TUI hlavní menu základních funkcí],
        image("pic/tui_main.png")
    )<tui-menu>],
    [#figure(
        caption: [TUI hlavní menu pokročilých funkcí],
        image("pic/tui_main_advanced.png")
)]
)
== Princip nastavení periferií<kap-perif>
Jelikož pouzdra SOP8 a TSSOP20 mají malý počet výstupů, není možné mít aktivované všechny periferie najednou. Další důvod je například využití časovače TIM2, který je jako jediný 32 bitový a je nutný k více funkcím. Jeden z příkladů je využití časovače TIM2 na `PA0` ke měření frekvence a zároveň využití TIM2 na generování pulzů. Další příklad je kolize periferie USART2 a kanálu `PB7`, kde je potřeba jiné nastavení pinů. Jeden z velkých problémů můžou činit DMA kanály, kterých je na STM32G030 pouze pět, což vyžaduje častou reinicializaci kvůli změně funkce @STM32G0-REF.

Sonda obsahuje velké množství nastavení periferií a je velice snadné ztratit přehled, která periferie je inicializovaná, a která není. Pro vyhnutí se tomuto problému během vývoje bylo zvoleno řešení, kdy při každém přepnutí funkce uživatelem jsou všechny periferie uvedeny do základního stavu a poté podle funkce zvolené uživatelem jsou nastaveny pouze konkrétní periferie nutné pro danou funkci. Tento způsob minimalizuje výskyt nedefinovaného chování, které během vývoje mohou nastat. Inicializace a deinicialiace periferií se řeší během obslužní smyčky, která při nastaveném příznaku `need_perif_update`, nastaví všechny periferie do původního stavu a následně dle zvolené funkce uživatelem je nastavena periferie. Nastavení příznaku je provedeno v případě, že sonda potřebuje po uběhnutém čase změnit nastavení, nebo pokud uživatel pomocí vstupu z UARTu vyvolá žádost o přepnutí funkce.

== Implementace měření s ADC
=== Měření napětí a logických úrovní <kap-volt>
Napětí je měřeno pomocí AD převodníku na dvou kanálech v případě SOP8 a na třech v případě TSSOP20 (+ kanál s referenčním napětím). Uživatel skrze TUI může vypnout či zapnout měření na určitém kanále. Jak bylo zmíněno v #ref(<kap-mereni>, supplement: [kapitole]), ADC průběžně měří $32$ vzorků za $320$~ms ($100$ Hz). Každé měření kanálu je nastaveno na 160 cyklů, což je maximální přesnost měření.

K časování měření je využito časovače TIM3, který po uplynutí času vyvolá přerušení a je naměřena hodnota ADC. Časovač má nastavenou předděličku na $64000-1$, což nastaví frekvenci časovače z $64$ MHz na $1$ kHz (neboli časovač inkrementuje hodnotu každou 1~ms). Jelikož ADC běží na frekvenci $32$ MHz a změření jednoho kanálu trvá $160$ cyklů, změření jednoho kanálu trvá $~5$ $mu$s. Protože frekvence měření je $100$ Hz, můžeme tuto hodnotu zanedbat a nastavit periodu časovače na $10 - 1$.

Při přetečení časovače je vyvoláno přerušení, které zavolá callback z #ref(<code-adc-callback>, supplement: [ukázky kódu]). Funkce zastaví časovač, a sekvenčně začne měřit poměrnou hodnotu mezi napětím na kanálu a $V_"dd"$. Po dokončení konverze je tato hodnota uložena do dynamicky alokovaného pole `voltage_measures` o velikosti $64 times "počet aktivních kanálů"$ #footnote[ADC při nastavení více kanálu sekvenčně prochází všechny kanály dokola.]. Toto pole se chová cyklicky, tzn. při překročení počtu prvků se začne plnit od začátku. Po změření všech kanálů je resetován a nastartován časovač.


#v(10pt)
#figure(
    placement: none,
    caption:[Naměření vzorku z ADC (ukázka bez ošetření)],
    supplement: [Úryvek kódu],
```C
void adc_measure_callback(adc_vars_t* adc_perif) {
    HAL_TIM_Base_Stop_IT(adc_perif->timer);
    
    HAL_ADC_Start(adc_perif->hadc);
    for (uint8_t i = 0; i < adc_perif->n_active_channels; ++i) {
        HAL_ADC_PollForConversion(adc_perif->hadc, ADC_DELAY);
        adc_perif->voltage_measures[adc_perif->measures_index++] = HAL_ADC_GetValue(adc_perif->hadc);
        if (adc_perif->measures_index >=
            adc_perif->n_active_channels * CHANNEL_NUM_SAMPLES) {
            adc_perif->measures_index = 0;
        }
    }

    HAL_ADC_Stop(adc_perif->hadc);
    __HAL_TIM_SET_COUNTER(adc_perif->timer, 0);
    HAL_TIM_Base_Start_IT(adc_perif->timer);
}
```
)<code-adc-callback>
#v(10pt)

Každých $320$ ms obslužní smyčka vezme naměřené vzorky z `voltage_measures` a udělá aritmetický průměr každého kanálu. Po provedení průměru je z poměrné hodnoty vypočítáno referenční napětí a napětí každého kanálu. K výpočtům napětí je, na základě vztahů z #ref(<adc>, supplement: [kapitoly]), využito makro z HAL knihovny (@code-calc-voltage). Toto napětí je poté zobrazeno dynamicky na stránce. U každého kanálu je také vyhodnocené zda naměřená hodnota napětí odpovídá log. "1", log, "0" nebo se napětí nachází v nedefinované oblasti#footnote[Podrobnosti je možné najít v manuálu použití.].
#figure(
    caption:[TUI měření napětí],
    image("pic/tui_voltmetr.png", width: 85%)
)<tui-voltmetr>

K měření napětí je doporučené, aby na vstupy měřících kanálů byly připojeny ochrané rezistory o velikosti $2.2$ $k Omega$. Tyto rezistory zapojené v sérii s ADC vstupem slouží jako *omezení proudu*, který může protékat do citlivého vstupního obvodu mikrokontroleru. okud dojde k náhodnému přepětí (např. připojení vyššího napětí než je maximální povolená hodnota nebo zkratu), rezistor sníží proud na bezpečnou úroveň a zabrání poškození ADC převodníku. Toto opatření může ochránit MCU před nesprávným zapojením obvodu nezkušeným uživatelem.


=== Měření odporu
Měření odporu vychází z principů měření z #ref(<kap-volt>, supplement:[kapitoly]). Pro změření odporu daných rezistorů, je změřeno napětí stejným způsobem jako v předchozí kapitole, ale pouze na prvním kanále obou pouzder. Z naměřených vzorků frekvencí $100$ Hz z kanálu 1 a referenčního napětí je spočítán průměr a poté je poměrová hodnota převedena na napětí. Z hodnoty napětí je poté, na základě normálového rezistoru, vypočítán odpor měřeného rezistoru (@code-getmeasure). V základním nastavení je normálový rezistor nastaven na $10$~$k Omega$. Toto nastavení může uživatel předefinovat dle jeho potřeb.

#figure(
    caption: [TUI měření odporu],
    image("pic/tui_ohm.png", width: 85%)
)<tui-ohm>

== Implementace měření frekvence a odchytávání pulzů


Stránka měření frekvence (@tui-freq) zobrazuje `Frequency`, což je měření metodou hradlování, `Reciprocial frequency`, což je frekvence naměřená skrze šířky pulzu, `High pulse width`/`Low pulse width`, což je spočítaná šířka nízkého a vysokého pulzu a `Duty` neboli střída v případě, že by to byl signál PWM. Pro výpočet těchto hodnot, se periodicky střídá měření hradlováním a reciproční měření. Hradlovací čas je možné nastavovat z předvybraných časů.

#figure(
    caption: [TUI měření frekvence],
    image("pic/tui_freq.png", width: 85%)
)<tui-freq>

=== Měření frekvence hradlováním
Při měření frekvence metodou hradlováním, jsou využity dva čítače. První časovač, TIM3, je zvolen, pro měření času hradlování. Tento čítač je nastavený předděličkou na $1$ kHz (incrementace každou milisekundu) a poté podle nastavené periody bude nastaven hradlovací čas. Za hradlovací čas lze volit hodnoty 50, 100, 200, 500 a 1000 milisekund. Jelikož TIM3 má promněnlivou periodu, tak je nastaven příznak `AutoReloadPreload`, který při změně periody za běhu časovače, je perioda aplikována až po přetečení časovače. Pokud by časovač měl hodnotu např. 500, a perioda by byla nastavena na 200, časovač by nemohl přetéct a nastal by problém. TIM3 je také nastaven jako One Pulse, což znamená, že při přetečení bude vypnut. Nastavení periferie je ukázáno v #ref(<code-tim3>, supplement: [úryvku kódu]).

Na počítání hran je využit 32 bitový TIM2, který z důvodu své velikosti může napočítat velké množství pulsů i s poměrně dlouhým hradlovacím časem. Aby TIM2 počítal hrany, lze nastavit pro časovač tzv. ETR, neboli externí trigger. Tento trigger inkrementuje časovač pokaždé, když na ETR pinu (`PA0`) bude hrana (v tomto nastavení náběžná). Časovač v podstatě ignoruje interní hodiny, které inkrementují každý cyklus časovač, ale řídí se podle externích hodin na pinu `PA0`, které jsou v případě sondy měřená frekvence @STM32G0-REF. @signal-freq-measure ukazuje způsob spolupráce TIM2 a TIM3.

Při vysokých frekvencích může nastat problém, kdy bude záležet i na rozdílu mezi zapnutím TIM2 a TIM3. Tyto dva časovače za normálních okolností musí být zahájeny sekvenčně a to vede k tomu, že buď bude nepatrně delší hradlovací čas a bude napočítáno více pulsů než by mělo. Design časovačů nicméně poskytuje řešení, kdy časovač může vyvolávat triggery a jiný časovač může na tyto triggery reagovat. V případě sondy byl TIM3 nastaven jako master timer, který při spuštění vyvolá trigger. TIM3 byl nastaven jako slave timer. Slave timer čeká, až dostane trigger a v momentě, kdy trigger dostane, začne počítat. Toto nastavení je ukázáno v #ref(<code-tim3>, supplement: [úryvku kódu]) a #ref(<code-tim2-freq>, supplement:[]).
#figure(
    caption: [Signály při měření frekvence hradlováním],
    image("pic/freq_etr.png")
)<signal-freq-measure>
Po změření pulsů se provede výpočet frekvence. Zde nastavá problém, protože získání frekvence je nutné dělit počet pulzů, hradlovým časem. Nicméně pro MCU je operace dělení poměrně náročná. Jelikož všechny hradlovací časy (kromě 1000 ms), jsou pod 1 sekundu, dojde k dělení desetiným číslem. Toto desetiné číslo je ale možné převést na zlomek a po úpravě vznikne vztah, ve kterém eliminujeme dělení. @n-t demostruje příklad na $500$ ms.
#v(10pt)
$ f_"gate" = N / T_"gate" = N / 0.5 = N / (1/2) = N / 1 times 2/1  = 2N $ <n-t>
#v(10pt)


Měření touto metodou bylo otestováno měření nižších desítek MHz a naměřená odchylka od původní hodnoty byla $~0.16$ $%$. Což pravděpodobně bude způsobeno tím, že sonda nevyužívá externího krystalu ale interního oscilačního obvodu @STM32G0-REF, tak může být lehká odchylka od reference. Při vyšších frekvencích zde můžou hrát roli i režie implementovaná pro časovače.



=== Reciproční měření frekvence
Při recipročním měření frekvence je využito časovače TIM2. Časovač má nastaveny celkově 2 kanály do módu input capture. Input capture kanálu, který při hraně na vstupu uloží aktuální hodnotu časovače do registru a následně metodou DMA do paměti. Důvod inicializace dvou kanálů místo jednoho je ten, že každý kanál sice monitoruje stejný pin, ale jeden reaguje na náběžnou a druhá na sestupnou. Kanál sice umí detekovat obě najednou, nicméně pro funkci měření je nutné rozpoznat, která hrana je náběžná a která je sestupná. U frekvence je vždy změřena náběžná hrana, poté sestupná hrana a poté opět náběžná. Důvod proč jsou měřena i sestupná hrana je určení střídy v případě PWM signálu. Pokud je naměřena tato posloupnost, je možné vypočítat reciproční frekvenci, střídu a šířku pulzů.

#figure(
    supplement: [Úryvek kódu],
    caption: [Funkce pro výpočet veličin na základě recipročního měření],
    placement: none,
```C
// Získání rozdílu mezi hranami
uint32_t high_delta = 
        edge_times[DET_EDGE2_FALL] - edge_times[DET_EDGE1_RISE];
uint32_t low_delta =
        edge_times[DET_EDGE3_RISE] - edge_times[DET_EDGE2_FALL];
// Výpočet času pulzu
detector->widths[DET_LOW_WIDTH] = (low_delta) / PROCESSOR_FREQ_IN_MHZ;
detector->widths[DET_HIGH_WIDTH] = (high_delta) / PROCESSOR_FREQ_IN_MHZ;
uint64_t period = (detector->widths[DET_LOW_WIDTH] + detector->widths[DET_HIGH_WIDTH]);
// ochrana před dělením nulou
if (period > 0) {
    detector->rec_frequency = 1000000 / period; // v ms
    detector->pwm_duty =
            ((uint64_t)detector->widths[DET_HIGH_WIDTH] * 100) / period;
}
```
)
#v(10pt)


=== Odchytávání pulzů

#v(10pt)
#figure(
    placement: none,
    caption: [TUI odchytávání pulzů],
    image("pic/tui_pulse.png", width: 80%)
)<tui-pulse>
#v(10pt)
Odchytávání pulzů je podfunkce na stránce měření frekvence. Při detekci náběžné nebo sestupné hrany (dle nastavení uživatele) je nastaven příznak, který je následně vykreslen na terminál. Uživatel tento příznak poté může smazat. K detekci pulzů je využito nastavení periferií jako při recipročním měření frekvence. Časovač, který je na vstupu nastaven jako input capture ukládá do registru hodnotu a je vyvoláno přerušení. Během přerušení je zavolána funkce, která zkontroluje, zda je to odpovídající hrana a pokud ano, je nastaven příznak. K odchytávání pulzů by mohlo být využito EXTI callbacku, nicméně to by znamenalo jiné nastavení periferií a zkomplikování sondy z hlediska vývoje.

== Implementace generování pulzů
Generování pulzů je realizováno za pomocí časovače TIM2, který má kanál 1 nastaven na PWM o střídě $50$ %. Předdělička časovače je nastavena na $64-1$, což znamená, že časovač běží na frekvenci $1$ MHz (inkrementace každou $1$ $mu$s). Poté pomocí periody čítače na uživatelem nastavována šířka pulzu, kdy zvolená šírka je vynásobena dvěma, protože při nastavení tomto nastavení dojde k přerušení pulzu v polovině periody. Aby časovač vyslal pouze jeden pulz, tak je inicializován tzv. One pulse, což způsobí zastavení časovače po jednom přetečení. Využití One pulse a časovače pro generaci je zajištěna velice přesná šířka jednoho pulzu.
$ f_"send" = 1/(2 times "perioda") $ <freq_pulse>
Funkce umí generovat jeden pulz, více pulzů a generovat neustále. Generace `X` pulzů funguje na principu One Pulse, kdy po dokončení jednoho pulzu je ihned zaslán další. Takto je možné poslat přesný počet pulzů za pomocí PWM. V případě neustále generace, je One Pulse deinicializován, a je vysílána frekvence odpovídající #ref(<freq_pulse>, supplement: [vztahu]). Pulz může být poslán jako pulz úrovně $3.3$ V a nebo jako pulz $0$ V. Nastavení časovače umožňuje nastavit pulz o šírky o řádech až mikrosekund. @gprah-pulse demonstruje přesnost, která byla naměřena pomocí osciloskopu. Generování pulzů může být vhodné pro testování čítačů, posuvných registrů a obvodů, kde je potřeba generovat hodinový signál.

#figure(
    caption: [Demonstrace pulzů],
    image("pic/pulse_graph.svg", width: 80%)
)<gprah-pulse>

== Implementace nastavování úrovní
#v(10pt)
#figure(
    placement: none,
    caption: [TUI nastavování úrovní],
    image("pic/tui_levels.png", width: 80%)
)<tui-levels>
#v(10pt)

Nastavování logických úrovní funguje celkem na 4 kanálech, které je možné skrze uživatelské rozhraní vypnou nebo zapnout dle potřeby. Každý pin je inicializován skrze STM32 HAL strukturu `GPIO_InitTypeDef`, kde je nastaven `GPIO_MODE_OUTPUT_PP`. Tento mód nastaví push pull na pin a lze ho tak ovládat skrze stisk tlačítka. @tui-levels ukazuje vizualizaci stránky pro funkci nastavování úrovní.

== Implementace diagnostiky posuvného registru
#v(10pt)
#figure(
    placement: none,
    caption: [Signály při měření frekvence hradlováním],
    image("pic/tui_register.png", width: 85%)
)<tui-register>
#v(10pt)

Funkce diagnostiky posuvného registru nabízí možnost nastavení jednotlivých bitů a následné odesílání dat do 8 bitového posuvného registru. Pro naplnění dat do posuvného registru je `PA7` připojen na jako hodinový signál registru a `PA0` jako datový pin do posuvného registru. Sonda umí posílat všech 8 bitů najednou a nebo je možné posílat bity postupně manuálně. Doba odesílání jednoho bitu je 200 ms. Tento čas nabízí možnost vizuální kontroly diagnostikovaného obvodu během odesílání. @code-shift ukazuje způsob odeslání jednoho bitu do posuvného registru.
Funkce nabízí i kompatibilitu s posuvnými registry `SNx4HC595`, která na základě signálu `RCLK` převede data z registru na paralelní výstupy @snx4hc595.

== Implementace diagnostiky Neopixel komunikace <kap-neopixel>
Neopixel má speciální způsob komunikace, který využívají RGB LED. Tato komunikace má svá specifika, rozebrána v #ref(<kap-neopixel-ter>, supplement: [kapitole]), která jsou nutná implementovat do sondy. Uživatel díky této funkci může odesílat barvy, která otestují funkčnost RGB LED, a pasivně monitorovat barvu, která se odesílá do RGB LED. Možností je také testování LED v serii, kde uživatel může odeslat barvu, dle pravidel komunikace, na všechny LED najednou. To je užitečné v případě, kdy již jsou LED zapojeny v serii a je nutné otestovat, zda není chyba u nějaké konkrétní LED.
=== Monitorování komunikace LED Neopixel 
Pro monitorování dat odeslané barvy přes seriovou komunikaci je využit časovač TIM2. Časovač má nastaveny 2 kanály v nastavení input capture, kde jeden reaguje na náběžnou hranu a druhý reaguje na sestupnou hranu. Tyto časy hran jsou zaznamenávány DMA metodou do paměti, kde sonda provede analýzu dat. Jelikož DMA je nastavené jako cirkulární buffer, začátek dat nutně nemusí být jako první prvek v poli. Pokud by například bylo odesláno méně bitů nebo by se na vodiči objevila parazitní hrana, začátek se posune. Proto algoritmus začne hledat začátek tak, že prochází pole a hledá, mezi jakou sestupnou a vzestupnou hranou je větší prodleva než odpovídá `Reset` času (viz. @code-neopixel-find).



#figure(
    supplement: [Úryvek kódu],
    caption: [Funkce pro nalezení začátku komunikace neopixel],
    placement: none,
```C
int8_t neopixel_find_start(neopixel_measure_t* data) {
    int8_t start_index = -1;
    uint8_t prev_measure = NEOPIXEL_DATA_LEN - 1;
    for (uint8_t i = 0; i < NEOPIXEL_DATA_LEN; ++i) {
        uint32_t curr_rise_edge = data->rise_edge[i];
        uint32_t prev_fall_edge = data->fall_edge[prev_measure];
        uint64_t width;
        // v případě, že časovač přetekl, přičti velikost časovače
        if (curr_rise_edge < prev_fall_edge) {
            width = (curr_rise_edge + 0xFFFFFFFF) - prev_fall_edge;
        } else {
            width = curr_rise_edge - prev_fall_edge;
        }
        if (width > NEOPIXEL_RESET) {
            // nalezen start
            start_index = i;
            break;
        }
    }
    return start_index;
}
    ```
)<code-neopixel-find>
#v(10pt)
Po nalezení začátku datové sekvence, je zkontrolováno následujících 24 pulzů zda jejich šířka odpovídá velikosti bitové jedničky nebo nuly. Pokud je nalezen pulz, který neodpovídá velikosti, jsou data zahozena a dále se nepokračuje. Nakonec jsou jednotlivé bity posunuty tak, aby odpovídali 8 bitovému číslu pro každou složku.
#v(10pt)
#figure(
    supplement: [Úryvek kódu],
    caption: [Způsob definování hodnoty bitu z hran neopixel],
    placement: none,

```C
pulse_width = data->fall_edge[i] - data->rise_edge[i];
if (pulse_width >= NEOPIXEL_MIN_LOW &&
    pulse_width <= NEOPIXEL_MAX_LOW) {
    bits_detected[i] = 0;
} else if (pulse_width >= NEOPIXEL_MIN_HIGH &&
    pulse_width <= NEOPIXEL_MAX_HIGH) {
    bits_detected[i] = 1;
} else {
    return;
}
```
)
#v(10pt)

=== Testovací signály pro komunikaci LED Neopixel
Pro přenos testovacích dat do Neopixelů je využit časovač TIM1, konfigurovaný v režimu PWM. Časovač pracuje s předem připraveným polem hodnot (šířek pulzů), které jsou přes DMA načítány do časovače a převáděny na PWM signál s odpovídajícími střídami. Po zadání číselných hodnot pro červenou, zelenou a modrou jako 8 bitová čísla (0-255), jsou jednotlivá čísla složek rozděleno na bity (celkem 24 bitů). Každý bit je poté převeden na střídu PWM signálu. 

Pro správně široký pulz je potřeba nastavit periodu časovače a poté určit, jaká střída bude reprezentovat log. 1 a který log. 0. Po analýze byla zvolena perioda $80-1$ což při frekvenci časovače 64 MHz, odpovídá přetečení každých $1.25$ $mu$s. To odpovídá délce doby odeslání jednoho bitu @NEOPIXEL-REF. Při testování RGB LED odesílání bylo zjištěno, že odeslání log. 1 odpovídá $2/3$ periody a log. 0 odpovídá $1/3$ periody.

Po převedení čísla složky odpovídající střídy, jsou střídy přesunuty do pole aby bylo možné zahájit činnost časovače. Během testování této metody docházelo k nepredikovatelnému chování, kdy při specifických barvách, neodpovídala odeslaná data a barva LED. Příčinou byla prodleva mezi vyprázdnění dat z bufferu DMA a zastavení časovače. Časovač má v CCR registru uloženo při jaké hodnotě časovače má přepnout výstup. Po dosažení hodnoty je aktivován trigger pro DMA a je načtena nová hodnota z bufferu do CCR registru @STM32G0-REF. Po vyprázdnění bufferu již  nedojde k aktualizaci CCR registru, ale časovač neustále běží. Z tohoto důvodu jsou na začátek a na konec pole přidány nuly, které po odeslání dat nastaví do CCR registru nulu než dojde k zastavení časovače.

#figure(
    placement: none,
    caption: [Neopixel signál bez ukončení a s ukončení nulou],
    image("pic/neopixel_signal1.png")
)
== Implementace diagnostiky UART komunikace
Diagnostika UART je implementována za pomocí vestavěné periferie USART2 a je inicializována podobným způsobem jako USART1 pro komunikaci s PC. Rozdíl je ten, že pro tuto diagnostický nástroj je nutné nastavovat baudrate, délku slova, paritu a počet stop bitů flexibilně aby uživatel mohl přizpůsobit nastavení svým potřebám. Pro nastavování uživatel využívá klávesy a po každém nastavení je periferie znovu inicializována s novým nastavením. Periferie využívá piny `PA2` a `PA3`.

Pro nastavení periferie je nutné vstoupit do editačního módu, kdy je periferie zastavena. Uživatelovi vyskočí hláška "Cannot start until edit mode" (viz. @tui-uart-read) aby věděl, že se v tomto módu nachází. Na základě toho se změní i dolní lišta, na které se nachází nápověda ovládání. V tomto módu uživatel nastaví nastaví klávesama např. baudrate, kde stisknutím čísla je číslo přidáno na poslední pozici a stiskem `X` je poslední pozice smazána. Po opuštění tohoto módu je periferie nastavena a může být využita.

=== Monitoring UART komunikace
Monitorování UART je realizováno za pomocí DMA, kdy po obdržení slova (7, 8 nebo 9 bitového) toto slovo uloženo do pole. DMA je v tomto případě nastaveno jako cirkulární. Toto pole je poté periodicky zobrazováno na terminál. Na terminál je zobrazen jak symbol, tak jeho číselná reprezentace aby uživatel, i při symbolu jako mezera, mohl vidět zda byl daný symbol zachycen. @tui-uart-read ukazuje způsob vypisování symbolů. Uživatel může symboly vyčistit klávesou `G`.

Výpis funguje způsobem scrollování, tzn. pokud se zaplní stránka, tak nejstarší symbol je smazán, jsou na stránce posunuty a nový se vykreslí jako nejnovější. Tohoto bylo dosaženo tak, že při vykreslování se zjistí DMA counter, který určuje na jakou poslední pozici periferie zapsala data. Jelikož je pole cirkulární, tak je poté iterované celé pole do doby, než se vrátí pointer na začátek. @code-uart-scroll ukazuje implementaci této vlastnosti.
#v(10pt)
#figure(
    placement: none,
    caption: [TUI UART monitorování],
    image("pic/tui_uart_read.png", width: 80%)
)<tui-uart-read>
#v(10pt)
=== Posílání testovacích symbolů na UART kanál
Diagnostika UARTu také odesílá testovací symboly, které si uživatel navolí. Celkově lze odeslat až 10 symbolů najednou. Tyto symboly jsou poté odeslány pomocí tlačítka `S` skze blokovací funkci `HAL_UART_Transmit`. Odesílaná data lze modifikovat v tzv. "Data edit mode", který dává možnost zapisovat konkrétní symboly klávesnicí jako data a poté přepnout kurzor na další. Takto je možné upravit všechny symboly sekvenčně. Kurzor je reprezentován jako text, který je podbarven zeleně. Uživatel si může zvolit, jaké množství symbolů chce celkově odeslat.
#v(10pt)
#figure(
    placement: none,
    caption: [TUI UART odesílání testovacích symbolů],
    image("pic/tui_uart_write.png", width: 80%)
)<tui-uart-write>
#v(10pt)

== Implementace diagnostiky I2C
Diagnostika I2C je funkce, která pomáhá k analýze komunikace mezi MCU a senzory. Funkce umí, detekovat slave zařízení na sběrnici, chovat se jako slave, chovat se jako master, umí bez zásahu monitorovat komunikaci mezi masterem a slavem a umí testování OLED displeje na bázi SSD1306. Tyto funkce ulehčují diagnostiku při práci se senzory a zkracují čas hledání problému, pokud například senzor posílá nekorektní data apod. Pro implementaci bylo využito zabudované I2C periferie, která je runtime přenastavována podle potřeby.
#v(10pt)
#figure(
    supplement: [Úryvek kódu],
    caption: [I2C inicializace periferie],
    placement: none,
```C
void i2c_init_perif(i2c_perif_t* i2c_perif) {
    i2c_perif->hi2c->Instance = I2C1;
    i2c_perif->hi2c->Init.Timing = 0x10B17DB5; // normal mode
    i2c_perif->hi2c->Init.OwnAddress1 = 
        (global_var.device_state == DEV_STATE_ADV_I2C_SLAVE)
            ? i2c_perif->slave_address << 1
            : i2c_perif->slave_address;
    i2c_perif->hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c_perif->hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_perif->hi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    if (HAL_I2C_Init(i2c_perif->hi2c) != HAL_OK) {
        Error_Handler();
    }
}
```
)
=== Skener adres
#v(10pt)
#figure(
    placement: none,
    caption: [TUI I2C skenování adres],
    image("pic/tui_i2c_scan.png", width: 80%)
)<tui-i2c-scan>
#v(10pt)
Skenování adres zobrazuje všechny adresy zařízeních, které se na sběrnici nachází. Skenování probíhá tak, že je funkcí `HAL_I2C_IsDeviceReady` zkontrolováno sekvenčně všech 127 adres a adresy, ze kterých je odpověď `ACK` jsou poté vykresleny do terminálu.
=== Master mód
#v(10pt)
#figure(
    placement: none,
    caption: [TUI UART odesílání testovacích symbolů],
    image("pic/tui_i2c_write_d.png", width: 80%)
)<tui-i2c-master>
#v(10pt)

Master mód je vhodný pro odesílání testovacích sekvencí senzoru. Uživatel může nastavit adresu slave zařízení na které chce odeslat data a také může nastavit read/write bit. V případě write uživatel může poslat až 10 bajtů dat najednou. Periferie nejprve odešle první rámec, který obsahuje adresu a read/write bit na který zařízení odpoví `ACK`. Dále sonda začne odesílat data dokud všechny data úspěšně neodešle, nebo do momentu, kdy nastane error. Tento error může být například z důvodu, že zařízení neodpoví (`NACK`) nebo z jiných důvodů.

Pokud uživatel nastaví read, může odeslat na slave zařízení 1 bajt (většinou adresu paměti, ze které chce master číst) a přijme data, která mu slave pošle. Tyto data jsou reprezentovány na displeji terminálu jako hexadecimalní hodnoty. @tui-i2c-master ukazuje stránku I2C diagnostiky s read master módem. Červený čtvereček ohraničuje bajt, který je poslán senzoru a zelený rámeček ohraničuje bajt, který byl přijat od slave zařízení. Pod hodnotami je zobrazené `I2C OK`, což značí, že komunikace proběhla v pořádku. Počet bajtů, které budou přečteny lze uživatelsky volit.

@code-i2c-master-read reprezentuje funkci, která se stará I2C komunikaci, při čtení. Nejprve je zavolána funkce `HAL_I2C_Master_Transmit`, která odešle data slave zařízení ve write módu. Zde je možné si povšimnout, že adresa musí být bitově posunuta. Důvod posunutí je přítomnost read/write bitu, které je na posledním místě bajtu. Pokud pokud dojde k problému během odesílání, odesílání ukončeno a je vypsána chyba na terminál. 

#v(10pt)
#figure(
    supplement: [Úryvek kódu],
    caption: [I2C master read funkce],
```C
void i2c_read_data_master(i2c_perif_t* perif) {
    if (perif->send_data) {
        perif->send_data = 0;
        HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(
            perif->hi2c, perif->slave_address << 1,
            perif->master_read_send_data, 1, PERIF_DELAY);

        if (ret == HAL_OK) {
            ret = HAL_I2C_Master_Receive(perif->hi2c,
                                         perif->slave_address << 1,
                                         perif->slave_received_data,
                                         perif->bytes_to_catch, PERIF_DELAY);
            if (ret == HAL_OK) {
                perif->send_status = I2C_SEND_SUCCESS;
            } else {
                perif->send_status = I2C_ERROR_RECIEVE;
            }
        } else {
            perif->send_status = I2C_ERROR;
        }
        ansi_print_i2c_error(ret, perif->hi2c);
    }
}
```
)<code-i2c-master-read>

Po úspěšném odeslání je opět odeslán rámec s adresou, ale tentokrát je nastaven bit jako read. Po odeslání rámce sonda čeká na odpověď o dané velikost bajtů, definovanou dobu. Tyto data jsou poté zobrazena na terminálu. V případě chyby je chyba vyparsována a na terminál je vypsána pravděpodobná příčina. Stejný princip odesílání dat je i v případě zapisování dat, ale pouze nedochází z vyžádání čtení.
=== Testování SSD1306
#figure(
    placement: none,
    caption: [TUI I2C testování displeje SSD1306],
    image("pic/tui_i2c_ssd1306.png", width: 80%)
)<tui-i2c-ssd1306>

Testování OLED displeje SSD1306 je funkce která, na nastavenou adresu uživatelem#footnote[Tato adresa je nastavena stejným způsobem, jako v Master a Slave módu], odešle sekvenci, která inicializuje potřebné parametry displeje a rozsvítí všechny pixely na displeji, aby uživatel mohl ověři, zda displej není poškozený. 


=== Monitoring
Monitoring I2C je podstatná část, která umí číst jednoduché komunikace mezi master a slave zařízením. Monitoring I2C komunikace není jednoduše realizovatelná na I2C periferii. Pokud periferie je nastavena jako slave zařízení, dojde sice ke čtení komunikace, ale také dojde k odpovídání, což je nežádaný efekt. I2C periferie STM32 nenabízí možnost monitorovat komunikaci. Aby byla komunikace I2C mohla být monitorována, bylo k tomu využito SPI rozhraní.

Propojení I2C masteru s SPI slave zařízením prostřednictvím hodinového signálu vyžaduje specifické nastavení parametrů SPI, aby bylo možné sladit odlišné vlastnosti obou rozhraní. V I2C zůstává hodinový signál (SCL) v klidovém stavu na vysoké logické úrovni (HIGH), což vyžaduje nastavení polarity SPI hodin (CPOL) na hodnotu 1. Tím se zajistí, že SPI slave zařízení bude v nečinnosti očekávat vysokou úroveň hodinového signálu, což odpovídá výchozímu stavu I2C. Dále je nutné řešit počáteční fázi komunikace: I2C zahajuje přenos tzv. start podmínkou, kdy datová linka (SDA) poklesne před poklesem SCL. Tato první hrana SCL nesmí být SPI slave interpretována jako platný hodinový takt. K jejímu ignorování slouží nastavení fáze SPI hodin (CPHA) na hodnotu 1, která způsobí, že data jsou čtena až na druhé hraně hodinového signálu. Tato kombinace (CPOL=1 a CPHA=1) odpovídá SPI módu 3 a umožňuje SPI zařízení správně reagovat na hodiny generované I2C masterem. V praxi se tak SPI slave synchronizuje s I2C hodinami až po start podmínce, kdy začne zpracovávat data z následujících taktů, například při zápisu I2C adresy nebo přenosu dat.

Po propojení hodinových signálů je nastaveno SPI rozhraní na 9 bitovová slova. Je to z důvodu, že I2C má slova 8 bitová, ale poté je k tomu přidán `ACK`/`NACK` bit, což je devátý. Na základě této metody je možné monitorovat I2C sběrnici za pomocí SPI rozhraní. Data jsou ukládána do pole za pomocí metody DMA. Po získání dat z rozhraní jsou data parsována a zobrazována na terminál podle kontextu dat. Monitor rozpozná, jestli se jedná o zápis nebo nebo čtení dat a podle toho přizpůsobí výpis. @tui-i2c-monitor tuto skutečnost ukazuje na výpisu z terminálové aplikace. @code-i2c-monitor-init ukazuje způsob inicializace SPI rozhraní.

#figure(
    caption:[TUI I2C monitoring],
    image("pic/tui_i2c_monitor.png", width: 80%)
)<tui-i2c-monitor>
== Implementace diagnostiky SPI
Diagnostika SPI je, podobně jako u I2C, velice podstatný nástroj pro debugování komunikace mezi komponenty a MCU jako například paměti, senzory atd. STM32 má integrovanou periferii pro komunikaci SPI. SPI diagnostika obsahuje funkce jako čtení komunikace, aktivní odesílání testovacích dat a testování SSD1306 displeje. Uživatelské ovládání je co nejvíce přiblíženo k ovládání I2C diagnostiky aby uživatel se jednodušše naučil používat sondu. Pro SPI je možné nastavit jakou fázi nebo jakou polaritu má `SCK` využívat při zpracování dat.
=== Master mód
SPI periferie je konfigurována jako master, což umožňuje uživateli plně řídit komunikaci se slave zařízením. V tomto režimu může uživatel jak zapisovat data do slave zařízení, tak z něj číst odpovědi. Při zápisu sonda podporuje odeslání až 10 bajtů dat zadaných uživatelem v hexadecimalním formátu. Data jsou poté odeslány pomocí blokovací funkce `HAL_SPI_Transmit`. Ta zajistí, že mikrokontrolér čeká na dokončení celého přenosu před dalším krokem. Pro čtení dat sonda nejprve odešle jeden bajt dat a následně pomocí blokovací funkce `HAL_SPI_Receive` zachytí odpověď slave zařízení. @code-spi-transmit ilustruje klíčové části této logiky a také způsob detekování chyb v průběhu odesílání. Blokovací funkce zjednodušují implementaci, ale vyžadují pečlivé časování, aby nedošlo k zablokování systému při delších operacích.


#v(10pt)
#figure(
    supplement: [Úryvek kódu],
    caption: [SPI transmit funkce],
    placement: none,
```C
void spi_transmit(spi_perif_t* perif) {
    if (!perif->send_data) {
        return;
    }

    perif->error = SPI_ERROR_SUCCESS;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // NSS
    if (!perif->read_bit) {
        // only write
        if (HAL_SPI_Transmit(perif->hspi,
                             (uint8_t*)perif->data,
                             perif->bytes_count,
                             PERIF_DELAY) != HAL_OK) {
            perif->error = SPI_ERROR_SEND;
        }

    } else {
        // write
        if (HAL_SPI_Transmit(perif->hspi,
                             perif->master_send_data,
                             1,
                             PERIF_DELAY) != HAL_OK) {
            perif->error = SPI_ERROR_SEND;
        }
        //read
        if (HAL_SPI_Receive(perif->hspi,
                            perif->data,
                            perif->bytes_count,
                            100) != HAL_OK) {
            perif->error = SPI_ERROR_RECEIVE;
        }
    }

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // NSS
}
```
)<code-spi-transmit>

#v(10pt)
#figure(
    placement: none,
    caption:[TUI SPI master mód],
    image("pic/tui_spi_master.png", width: 78%)
)<tui-spi-master>
#v(10pt)

=== Monitoring
Sonda využívá SPI periferii konfigurovanou v slave režimu pro pasivní zachytávání dat z externího master zařízení. Uživatel může (podobně jako v master módu) nastavovat parametry hodinového signálu SCK (polaritu a fázi), aby sladil časování s požadavky systému. Pro zachycení dat, i při vysokých hodinových frekvencích, je implementováno DMA, které průběžně ukládá přijaté bajty přímo do vyhrazeného bufferu v paměti, aniž by zatěžoval jádro procesoru. Obslužná smyčka následně v pravidelných intervalech načte tato data z bufferu, převede je do hexadecimálního formátu a zobrazí na terminálovou aplikaci.  Tento přístup minimalizuje riziko ztráty bajtů i při maximální rychlosti SPI komunikace a zároveň udržuje responzivitu uživatelského rozhraní.

#v(10pt)
#figure(
    placement: none,
    caption:[TUI SPI monitor mód],
    image("pic/tui_spi_monitor.png", width: 78%)
)<tui-spi-slave>


= Návrh lokálního režimu STM32
== Logika nastavení režimů<kap-log-rezim>
Po připojení napájení MCU provede kontrolu pinu `BOOT0`. Pokud je pin `BOOT0` v nízké logické úrovni, firmware se načte z flash paměti a okamžitě spustí. Při spuštění firmwaru sondy, proběhne inicializace globalních struktur, které jsou nezbytné pro chod celé sondy. Globální struktura poskytuje potřebná data různým periferiím, které například periferie využívají při přerušeních. Po inicializaci struktury, která je deklarována #ref(<code-global_vars_t>, supplement:[v úryvku kódu]), dojde k inicializaci všech potřebných periferií, které dále jsou rozebrány v #ref(<kap-perif>, supplement: [kapitole]).

Po inicializaci periferií, sonda zkontroluje stav pinu `PA10` (remapovaném na `PA12`) na kterém se nachází `Rx` `USART1` periferie. Jak bylo zmíněno v #ref(<uart>, supplement:[kapitole]), pokud jsou dvě zařízení propojené a neprobíhá žádná komunikace, tak se na vodičích od `Tx` do `Rx` nachází logicky vysoká úroveň. Takto snda dokáže určit, zda je sonda připojena UART/USB převodníkem k PC, nebo je sonda pouze napájena např. skrze jiné MCU.
@sop8-hw a @tssop20-hw má v zapojení, na pinu `PA10`, rezistor o velikosti $10$ $K Omega$, který při nepřipojeném vodiči uzemní `Rx`. @dia-init prezentuje způsob inicializace. Po zvolení režimu, zařízení přejde do různého nastavení, které jsou nutné pro fungování režimu. Opětovné nastavení režimu opět dojde při dalším bootu sondy, protože jednotlivé režimy běží v nekonečném cyklu dokud je zařízení napájeno.

#v(10pt)
#figure(
    placement: none,
    caption: [Diagram nazpůsobu načítání režimů],
    diagram(
	    cell-size: (8mm, 10mm),
	    edge-stroke: 1pt,
	    edge-corner-radius: 5pt,
	    mark-scale: 70%,
        blob((0,0), [Start\ programu], tint: yellow, extrude: (0, 3)),
        edge("-|>"),
        blob((0,0.9), [Inicializace\ datové\ struktury], tint: yellow),
        edge("-|>"),
        blob((0,1.8), [Inicializace\ periferií], tint: yellow),
        edge("-|>"),
        blob((0,2.7), [ Vysoká úroveň\ na pinu `PA10`?], tint: red, shape: shapes.hexagon),
	    edge("rr", "=>", [Ano], label-pos: 0.4),
	    edge("ll", "=>", [Ne], label-pos: 0.4),
        blob((1.5,2.7), [Terminálový režim], tint: green, extrude: (1, 3)),
        edge("r,u,l,d", "--|>"),
        blob((-1.5,2.7), [Lokální režim], tint: green, extrude: (1, 3)),
        edge("l,u,r,d", "--|>"),
    )
)<dia-init>
#v(10pt)
#pagebreak(weak:true)
== Ovládání lokálního režimu
Jak #ref(<cil>, supplement: [kapitola]) zmiňuje, lokální mód je provozní režim, v němž zařízení nekomunikuje s externím počítačem a veškerá interakce s uživatelem probíhá výhradně prostřednictvím tlačítka a RGB LED diody. Tento režim je vhodný pro prvotní rychlou diagnostiku logického obvodu. Režim se ovládá skrze tlačítko a informace jsou zobrazovány prostřednictvím RGB LED WS2812. Lokální režim běží ve smyčce, kdy se periodicky kontrolují změny a uživatelské vstupy. Způsob zobrazování barev na WS2812 je popsán v #ref(<kap-neopixel>, supplement: [kapitole]).

#v(10pt)
#figure(
    caption: [Diagram způsobu reakce na vstupy uživatele v lokálním módu],
    placement: none,
    diagram(
        cell-size: (8mm, 10mm),
        edge-stroke: 1pt,
        edge-corner-radius: 5pt,
        mark-scale: 70%,
        blob((0,0), [Start], tint: yellow, extrude: (0, 3)),
        edge("-|>"),
        blob((0,1), [Jak bylo\ stisknuto\ tlačítko?], shape: shapes.hexagon, tint: red),
        edge((0,2), "=>", [Krátce]),
        edge((-1,2),"=>", [Dlouze]),
        edge((1,2),"=>", [Dvojitě]),

        blob((-1,2), [Přepnout\ aktuální stav], tint: yellow),
        edge((0,3), "-|>"),
        blob((0,2), [Reagovat\ na krátký stisk], tint: yellow),
        edge((0,3), "-|>"),
        blob((1,2), [Reagovat\ na dvojitý stisk], tint: yellow),
        edge((0,3), "-|>"),
        blob((0,3), [Rozsviť LED \dle výsledků periferie], tint: yellow),
        edge((1,3), "-|>"),
        blob((1,3), [Čekej], tint: orange, extrude: (2,4)),
        edge((1,3), (1.7,3), (1.7,2), (1.7, 1), "ll", "--|>"),
    )
)
#v(10pt)

Při zmáčknutí tlačítka dojde k přerušení a je zavolána funkce z #ref(<code-exti>, supplement:[úryvku kódu]), kde je zaznamenán čas zmáčknutí. Po uvolnění tlačítka dojde k přerušení náběžné hrany, a je zavolána funkce z #ref(<code-exti>, supplement: [úryvku kódu]), kde je zaznamenán čas uvolnění a následně funkce `extern_button_check_press`, z #ref(<code-extern_button>, supplement:[úryvku kódu]), porovná časy s referencí a určí, o který stisk se jedná. Funkce nastaví příznak v globální struktuře a v hlavní smyčce se poté provede příslušná akce. Tato metoda dokáže eliminovat nechtěné kmity tlačítka při stisku a uvolnění, kdy MCU zaznamenává velký počet hran v krátký moment (bouncing tlačítka).

Zařízení skrze tlačítko rozpozná tři interakce: _krátký stisk_ slouží k přepínání logických úrovních na určitém kanálu, _dvojitý stisk_ umožňuje cyklické přepínání mezi měřícími kanály, zatímco _dlouhý stisk_ (nad 500 ms) zahájí změnu stavu. Při stisku tlačítka je signalizováno změnou barvy LED na 1 sekundu, kde barva určuje k jaké změně došlo. Tyto barvy jsou definovány v uživatelském manuálu přiložený k této práci. 



#figure(
    caption: [Přerušení zavolané při zmáčknutí tlačítka],
    supplement: [Úryvek kódu],
    ```C
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
    // Pokud došlo k přerušení na pinu tlačítka
    if (GPIO_Pin == global_var.button_data->pin) {
        button_data_t* button_data = global_var.button_data;
        // zaznamenej čas, pokud tlačítko ještě nebylo stisknuto
        if (!button_data->is_pressed) {
            button_data->rise_edge_time = HAL_GetTick();
            button_data->is_pressed = true;
        }
    }
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
    // Pokud došlo k přerušení na pinu tlačítka
    if (GPIO_Pin == global_var.button_data->pin) {
        button_data_t* button_data = global_var.button_data;
    
        if (button_data->is_pressed) {
            // zaznamenej čas puštění tlačítka a zkontroluj
            // délku stisknutí
            button_data->fall_edge_time = HAL_GetTick();
            extern_button_check_press(button_data);
            button_data->is_pressed = false;
        }
    }
}
    ```
)<code-exti>



== Funkce lokálního režimu
Lokální režim má celkově 4 různé stavy, pro rychlou diagnostiku logického obvodu. Tyto funkce vychází ze standartních schopností komerečních logických sond. Stavy jsou přepínány dlouhým stiskem. 

=== Funkce logické sondy
Při zapnutí zařízení se vždy nastaví stav *logické sondy*. Tento stav čte na příslušném kanálu periodicky, jaká logická úroveň je naměřena AD převodníkem. Jsou využity piny `PB7` a `PA0`. Logickou úroveň je možné číst také jako logickou úroveň na GPIO, nicméně to neumožňuje rozlišit stav, kdy logická úroveň je v neurčité oblasti. Pomocí měření napětí na pinu lze zjistit zda napětí odpovídá CMOS logice či nikoliv. Pokud na pinu se nachází vysoká úroveň, LED se rozsvítí zeleně, v případě nízké úrovně se rozsvítí červená a pokud je napětí v neurčité oblasti, LED nesvítí. Tlačítkem poté lze přepínat mezi jednotlivými kanály. Tento stav vychází z terminálové funkce pro měření napětí a následné zjistění logiky. Tento stav je vhodný pro rychlé zjistění úrovně na vodiči při diagnostice obvodu.

=== Funkce nastavení logických úrovní
Funkce nastavení logických úrovní je stav, který po stisku tlačítka změní logickou úroveň na opačnou, tzn. pin je nastaven jako push-pull a pokud je na pinu nízká úroveň, změní se na vysokou a naopak. Tato úroveň lze nezávisle měnit na pinech `PB7` a `PA0`. RGB LED poté barvou reprezentuje, v jakém stavu je pin nastaven. Při vstoupení do tohoto stavu uživatelem, jsou piny inicializovány funkcí z #ref(<code-local-levels>, supplement: [úryvku kódu]). Tento stav je vhodný pro diagnostiku obvodu čítače, nebo klopných obvodů.

=== Funkce generování periodických pulzů
Funkce generování periodických pulzů je stav, kdy po stisku tlačíka je zapnuto na pinu `PA0` signál o frekvenci 1 Hz. Při zapnutí signálu na výstupu se rozsvítí RGB LED, aby si uživatel byl vědom aktivace. Po opětovném stisku tlačítka je generování signálu vypnuto. Takto nízká frekvence poskytuje možnost debugovat obvod, kde se například nachází posuvné registry nebo čítače. Uživatel například může vyzkoušet čítač, který zobrazuje čísla na sedmisegmentovém displeji a kontrolovat, zda se číslice mění korektně. Při vyšších frekvencích by toto znamenalo problém pro uživatele.

=== Funkce detekce pulzů
Detekce pulzu je stav, kdy je sledován pin `PA0` a detekována náběžná nebo sestupná hrana (detekce náběžné nebo sestupné hrany lze nastavit pomocí tlačítka). Pokud sonda detekuje hranu, rozsvítí RGB LED na 1 sekundu. Tento stav je vhodný pro detekci rychlého pulzu bez nutnosti použití osciloskopu. Sonda totiž detekuje velice krátkou hranu, ale uživatel ví, že pulz v obvodu byl, protože LED tento signál prodloužila.


#figure(
    caption:[Závislost mezi detekcí hrany a LED signalizace lokální režim],
    image("pic/pulse_dec.png", width: 80%)
)

Detekce funguje za pomocí časovače TIM2, který má nastavený kanál na režim input capture. Tento řežim při detekci hrany, uloží stav časovače do registru a je vyvoláno přerušení. Přerušení poté nastaví pomocný příznak, který bude zpracován při dalším cyklu smyčky. Obslužní smyčka poté rozsvítí LED. Důvod zvolení časovače místo přerušení EXTI, které vyvolá přerušení při hraně na vstupu, je ten, že tato metoda vychází z měření frekvencí v terminálovém režimu a tento způsob šetří paměť, která je na tomto MCU velice omezená.
#figure(
    caption: [Zachytávání pulzů v lokálním režimu],
    supplement: [Úryvek kódu],
```C
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance != TIM2) {
        return;
    }

    _Bool is_channel_1 = htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1;
    _Bool is_channel_2 = htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2;
    _Bool is_rise_edge_mode =
        global_var.detector->mode == DETECTOR_MODE_RISE_EDGE
    _Bool is_rise_edge_mode =
        global_var.detector->mode == DETECTOR_MODE_FALL_EDGE;

    if ((is_channel_1 && is_rise_edge_mode) ||
        (is_channel_2 && is_fall_edge_mode)) {
        detector->one_pulse_found = true;
    }
}
```
)

= Návrh omezené verze na RPI Pico
Návrh omezené verze na Rasberry Pi Pico #todo[dopsat]
== Komunikace s PC
== Měření napětí a logických úrovní
== Měření frekvence a detekce pulzů
== Generování pulzů



= Závěr a zhodnocení

#start-appendix(lang: "cs")[
= Citace
#bibliography("bibliography.bib")

= Dodatečné úryvky kódu
#figure(
    supplement: [Úryvek kódu],
    caption: [Struktura globálních promněných],
    placement: none,
```C
typedef struct {
    _Bool booted; // true pokud uživatel připojil seriovou komunikací
    dev_setup_t device_setup; // Lokální/Terminálový mód
    dev_state_t device_state; // měřící mód (napětí, odpor atd.)
    local_state_t local_state; // lokální režim měřící mód
    local_substate_t local_substate; // lokální režim měřený kanál
    unsigned char received_char; // Znak jako vstup
    _Bool need_frontend_update; // příznak aktualizace TUI
    _Bool need_perif_update; // příznak nastavení periferií
    ansi_page_type_t current_page; // aktualní frontend stránka

    // struktury pro základní měření
    adc_vars_t* adc_vars;
    sig_detector_t* signal_detector;
    sig_generator_t* signal_generator;

    // struktury pro vstupy/výstupy uživatele
    visual_output_t* visual_output;
    button_data_t* button_data;

    // struktury pro pokročilé funkce
    neopixel_measure_t* adv_neopixel_measure;
    shift_register_t* adv_shift_register;
    uart_perif_t* uart_perif;
    i2c_perif_t* i2c_perif;
    spi_perif_t* spi_perif;
} global_vars_t;
```
)<code-global_vars_t>

#figure(
    supplement: [Úryvek kódu],
    caption: [Funkce pro vykreslení barevného textu],
    placement: none,
```C
void ansi_send_text(const char* str,
                    const ansi_text_config_t* text_conf) {
    char buffer[TEXT_SEND_BUFF_SIZE];
    // offset stringu pro přidávání textu
    size_t offset = 0;     
    // Při nastavení barvy přidej ansi sekvenci pro zbarvení
    if (strlen(text_conf->bg_color) != 0) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%s",
                           text_conf->bg_color);
    }
    
    // Při nastavení barvy přidej ansi sekvenci pro zbarvení
    if (strlen(text_conf->color) != 0) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%s",
                           text_conf->color);
    }

    // Nastav text tučně
    if (text_conf->bold) {
        offset +=
            snprintf(buffer + offset, sizeof(buffer) - offset, "%s", BOLD_TEXT);
    }

    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%s", str);
    
    // Kontrola velikosti bufferu
    if (offset >= sizeof(buffer)) {
        PrintError("Buffer overflow in text");
        return;
    }
    ansi_send_string(buffer);
    // Escapování všech nastavení na konci stringu
    ansi_clear_format(); 
}
```
)<code-uart_text>

#figure(
    supplement: [Úryvek kódu],
    caption: [Ovládání sondy skrze symboly],
    placement: none,

```C
void get_current_control(void) {
    char received_char = global_var.received_char;
    switch (global_var.current_page) {
        case ANSI_PAGE_MAIN:
            control_main_page();
            break;
        case ANSI_PAGE_MAIN_ADVANCED:
            control_advanced_main_page();
            break;
        case ANSI_PAGE_VOLTAGE_MEASURE:
            control_voltage_page(received_char);
            break;
        case ANSI_PAGE_FREQUENCY_READER:
            control_frequency_reader_page(received_char, global_var.signal_detector);
            break;
        case ANSI_PAGE_IMPULSE_GENERATOR:
            control_impulse_generator_page(received_char);
            break;
        case ANSI_PAGE_LEVELS:
            control_levels_page(received_char);
            break;
        case ANSI_PAGE_NEOPIXEL_MEASURE:
            control_neopixel_measure_page(received_char);
            break;
        case ANSI_PAGE_SHIFT_REGISTER:
            control_shift_register_page(received_char);
            break;
        case ANSI_PAGE_UART:
            control_uart_page(received_char);
            break;
        case ANSI_PAGE_I2C:
            control_i2c_page(received_char);
            break;
        case ANSI_PAGE_SPI:
            control_spi_page(received_char);
            break;
        default:
            control_main_page();
    }
}
```
)<code-uart-parse-page>
#figure(
    supplement: [Úryvek kódu],
    caption: [Ovládání menu],
    placement: none,

```C
    void control_main_page(void) {
    switch (global_var.received_char) {
        case 'v':
        case 'V':
            ansi_set_current_page(ANSI_PAGE_VOLTAGE_MEASURE);
            dev_mode_change_mode(DEV_STATE_VOLTMETER);
            break;
        case 'f':
        case 'F':
            ansi_set_current_page(ANSI_PAGE_FREQUENCY_READER);
            dev_mode_change_mode(DEV_STATE_FREQUENCY_READ);
            break;
        case 'g':
        case 'G':
            ansi_set_current_page(ANSI_PAGE_IMPULSE_GENERATOR);
            dev_mode_change_mode(DEV_STATE_PULSE_GEN);
            break;
        case 'l':
        case 'L':
            if (NOT_SOP) {
                ansi_set_current_page(ANSI_PAGE_LEVELS);
                dev_mode_change_mode(DEV_STATE_LEVEL);
            }
            break;
        case 'a':
        case 'A':
            if (NOT_SOP) {
                ansi_set_current_page(ANSI_PAGE_MAIN_ADVANCED);
                dev_mode_change_mode(DEV_STATE_NONE);
            }
            break;
    }
}
```
)<code-uart-parse-menu>

#figure(
    supplement: [Úryvek kódu],
    caption: [Inicializace TIM3 jako časovač],
    placement: none,

```C
static void MX_TIM3_Init(void) {
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 64000 - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000 - 1; // základní nastavení které bude změněno
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_SINGLE) != HAL_OK) {
        Error_Handler(); // Časovač se sám zastaví po přetečení
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE; // trigger pro slave časovač
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) !=
        HAL_OK) {
        Error_Handler();
    }
}
```
)<code-tim3>

#figure(
    supplement: [Úryvek kódu],
    caption: [Využití HAL maker pro převod poměrových hodnot na napětí],
    placement: none,

```C
uint32_t adc_get_voltage(const uint32_t v_ref,
                         const uint32_t measure) {
    return __HAL_ADC_CALC_DATA_TO_VOLTAGE(v_ref, measure,
                                          ADC_RESOLUTION_12B);
}

uint32_t adc_get_v_ref( const uint32_t raw_voltage_value) {
    return __HAL_ADC_CALC_VREFANALOG_VOLTAGE(raw_voltage_value,
                                             ADC_RESOLUTION_12B);
}
```
)<code-calc-voltage>

#figure(
    placement: none,
    caption: [Způsob výpočtu odporu],
    supplement:[Úryvek kódu],
    ```C
uint32_t ref_voltage = adc_get_v_ref(adc_ch->avg_voltage[0]);
uint32_t measured_voltage = adc_get_voltage(ref_voltage,
                                            adc_ch->avg_voltage[1]);
uint32_t resistance = (adc_ch->base_resistor * measured_voltage) 
                / (ref_voltage - measured_voltage);
    ```
)<code-getmeasure>

#figure(
    supplement: [Úryvek kódu],
    caption: [Inicializace TIM2 jako čítač hran],
    placement: none,

```C
void timer_setup_slave_freq(void) {
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_SlaveConfigTypeDef sSlaveConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    slave_tim->Instance = TIM2;
    slave_tim->Init.Prescaler = 0; // nastaven na maximum
    slave_tim->Init.CounterMode = TIM_COUNTERMODE_UP;
    slave_tim->Init.Period = 4294967295;
    slave_tim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    slave_tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(slave_tim) != HAL_OK) {
        Error_Handler();
    }
    // inicializace externího clocku
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
    sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
    sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
    sClockSourceConfig.ClockFilter = 0;
    if (HAL_TIM_ConfigClockSource(slave_tim, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }

    // nastavení závislosti spuštění na TIM3
    sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
    sSlaveConfig.InputTrigger = TIM_TS_ITR2;
    if (HAL_TIM_SlaveConfigSynchro(slave_tim, &sSlaveConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(slave_tim, &sMasterConfig) !=
        HAL_OK) {
        Error_Handler();
    }
}
```
)<code-tim2-freq>
#v(10pt)
#figure(
    supplement: [Úryvek kódu],
    caption: [Způsob zápisu jednoho bitu do posuvného registru],
    placement: none,
```C
void shift_register_send_one_signal(shift_register_t* shift_register,
                                    const uint8_t index) {
    if (shift_register->bits[index]) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    }
    HAL_Delay(SHIFT_REGISTER_LATCH);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
    HAL_Delay(SHIFT_REGISTER_LATCH);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
    HAL_Delay(SHIFT_REGISTER_LATCH);
}
```
)<code-shift>


#v(10pt)
#figure(
    supplement: [Úryvek kódu],
    caption: [Způsob vykreslování UART symbolů scrollováním],
    placement: none,
```C

void ansi_render_read_vals(uart_perif_t* uart) {
    // získání pozice začátku
    uint16_t curr_buff_index =
        UART_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(uart->huart->hdmarx);

    char buff[11];
    uint8_t col = 0;
    uint8_t row = 12;
    ansi_set_cursor(row, 5);

    for (size_t i = 0; i < UART_BUFFER_SIZE; ++i) {
        if (curr_buff_index >= UART_BUFFER_SIZE) {
            curr_buff_index = 0;
        }
        if (uart->received_char[curr_buff_index] == 0) {
            ++curr_buff_index;
            continue;
        }

        ++col;
        snprintf(buff, 11, " %c(%3d)", uart->received_char[curr_buff_index],
                 uart->received_char[curr_buff_index]);
        ansi_send_text(buff, &ansi_default_conf);

        if (col >= 10) {
            col = 0;
            ++row;
            ansi_set_cursor(row, 5);
        }
        ++curr_buff_index;
    }
}
```
)<code-uart-scroll>

#v(10pt)
#figure(
    supplement: [Úryvek kódu],
    caption: [Inicializace SSD1306 displeje I2C],
    placement: none,

```C
uint8_t init_sequence[] = {0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3,
                           0x00, 0x40, 0x8D,0x14, 0x20, 0x02,
                           0xA0, 0xC0, 0xDA, 0x12, 0x81, 0xCF,
                           0xD9, 0xF1, 0xDB, 0x40, 0xA5, 0xA6, 0xAF};

HAL_StatusTypeDef SSD1306_spi_init_display(SPI_HandleTypeDef* hspi) {
    HAL_StatusTypeDef status;

    for (uint8_t i = 0; i < sizeof(init_sequence); i++) {
        if ((status = SSD1306_spi_write_command(hspi, init_sequence[i]))) {
            return status;
        }
        HAL_Delay(40);
    }
    return HAL_OK;
}
```
)<code-i2c-ssd1306>

#figure(
    supplement: [Úryvek kódu],
    caption: [I2C inicializace periferie pro monitoring],
    placement: none,

```C
void i2c_monitor_init(i2c_perif_t* i2c_perif, const spi_perif_t* spi_perif) {
    spi_perif->hspi->Instance = SPI1;
    spi_perif->hspi->Init.Mode = SPI_MODE_SLAVE;
    spi_perif->hspi->Init.Direction = SPI_DIRECTION_2LINES;
    spi_perif->hspi->Init.DataSize = SPI_DATASIZE_9BIT;
    spi_perif->hspi->Init.CLKPolarity = SPI_POLARITY_HIGH;
    spi_perif->hspi->Init.CLKPhase = SPI_PHASE_2EDGE;
    spi_perif->hspi->Init.NSS = SPI_NSS_SOFT;
    spi_perif->hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi_perif->hspi->Init.TIMode = SPI_TIMODE_DISABLE;
    spi_perif->hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi_perif->hspi->Init.CRCPolynomial = 7;
    spi_perif->hspi->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    spi_perif->hspi->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    if (HAL_SPI_Init(spi_perif->hspi) != HAL_OK) {
        Error_Handler();
    }
    memset(i2c_perif->monitor_data, 0, I2C_ARRAY_SIZE);
    gpio_spi_slave_init();
    HAL_SPI_Receive_DMA(spi_perif->hspi, (uint8_t*)i2c_perif->monitor_data,
                        I2C_ARRAY_SIZE * 2);
}
```
)<code-i2c-monitor-init>

#figure(
    supplement: [Úryvek kódu],
    caption: [Způsob detekce boucingu],
    placement: none,

```C
void extern_button_check_press(button_data_t* data) {
    uint32_t time = data->fall_edge_time - data->rise_edge_time;
    
    // identifikace typu stisknutí
    if (time > SHORT_PRESS_TIME && time < LONG_PRESS_TIME) {
        data->short_press = true;
        data->long_press = false;
        uint32_t curr_time = HAL_GetTick();
        
        // detekce druhého krátkého stisknutí
        if (curr_time - data->last_short_button_time < DOUBLE_PRESS_TIME &&
            !data->double_press) {
            data->double_press = true;
            data->short_press = false;
        } else {
            data->last_short_button_time = curr_time;
        }

    } else if (time > LONG_PRESS_TIME && !data->long_press) {
        data->long_press = true;
        data->short_press = false;
        data->double_press = false;
    }
}
```
)<code-extern_button>
#figure(
    supplement: [Úryvek kódu],
    caption: [Inicializace pinů lokálního režimu pro stav logických úrovní],
    placement: none,

```C
void gpio_init_push_pull(void) {
    gpio_deinit_pins();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}
```
)<code-local-levels>
= Uživatelská příručka
]
