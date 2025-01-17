#let title-page(
  print,
  title: "",
  author: (
    name: "",
    email: "",
    url: "",
  ),
  submission-date: datetime.today(),
  bachelor: true,
  supervisor: "",
  faculty: "",
  department: "",
  study-programme: "",
  branch-of-study: "",
) = {
  // render as a separate page
  // inner margin is 8mm due to binding loss, but without
  //  the bent page extra, which is not an issue for the title page
  let inside-margin = if print {8mm} else {0mm}
  show: page.with(margin: (top: 0mm, bottom: 0mm, inside: inside-margin, outside: 0mm))

  set align(center)
  set place(center)
  set text(font: "Technika", weight: "extralight", size: 10.3pt)

  // shorthand to vertically position elements
  let b(dy, content, size: none, weight: none) = {
    set text(size: size) if size != none
    set text(weight: weight) if weight != none
    place(dy: dy, text(content))
  }

  b(33mm)[
    České vysoké učení technické v Praze \
    #faculty \
    #department
  ]

  b(63.5mm)[
    #image("./res/symbol_cvut_konturova_verze_cb.svg")
  ]

  b(131.5mm, size: 12.5pt)[
    #if bachelor [
      Semestrální práce
    ] else [
      Master's Thesis
    ]
  ]

  b(140.7mm, size: 14.8pt, weight: "regular")[
    #title
  ]
  
  b(154.25mm, [
    #text(size: 12.5pt, style: "italic")[#author.name] \

    \
    #author.email \
    #link(author.url)
  ])

  b(210mm)[Vedoucí práce: #supervisor]

  b(248.2mm)[Studenský program: #study-programme]
  
  b(254.3mm, [#submission-date.display("leden [year]")])
}


#let abstract-page(
  submission-date,
  //abstract-en: [],
  //abstract-cz: [],
  acknowledgement: [],
  declaration: [
    Prohlašuji, že jsem předloženou práci vypracoval samostatně a že jsem uvedl veškeré použité informační zdroje vsouladu s Metodickým pokynem o dodržování etických principů při přípravě vysokoškolských závěrečných prací.
    
    Beru na vědomí, že se na moji práci vztahují práva a povinnosti vyplývající ze zákona č. 121/2000 Sb., autorského zákona, ve znění pozdějších předpisů v~souladu s ust. § 2373 odst. 2 zákona č. 89/2012 Sb., občanský zákoník, ve~znění pozdějších předpisů, tímto uděluji nevýhradní oprávnění (licenci) k užití této mojí práce, a to včetně všech počítačových programů, jež jsou její součástí či přílohou a veškeré jejich dokumentace (dále souhrnně jen „Dílo“), a to všem osobám, které si přejí Dílo užít. Tyto osoby jsou oprávněny Dílo užít jakýmkoli způsobem, který nesnižuje hodnotu Díla a za jakýmkoli účelem (včetně užití k výdělečným účelům). Toto oprávnění je časově, teritoriálně i množstevně neomezené.
  ]
) = {
  // render as a separate page; add room at the bottom for TODOs and notes
  show: page.with(margin: (bottom: 0mm))
  
  set heading(outlined: false, bookmarked: false)
  // pretty hacky way to disable the implicit linebreak in my heading style
  show heading: it => {
    show pagebreak: it => {linebreak()}
    block(it)
    //block(it, above: 2pt)
  }

  // no idea why there is a margin here
  v(-30.2pt)
  [
    //= Abstract
    //#abstract-en
  ]
  
  [
   // = Abstrakt (CZ)
   // #abstract-cz
  ]

  v(6.6pt)
  //v(-6pt)
  grid(columns: (47.5%, 47.5%), gutter: 5%,
    [
      = Poděkování
      #set text(style: "italic", lang: "cs")
      #acknowledgement
    ],
  
    [
      = Prohlášení
      #set text(lang: "cs")
      #declaration
      
      V Praze, #submission-date.display("[day]. [month]. [year]")

      #v(2em)
      #repeat[.]
    ],
  )

  context {
    set text(size: 20pt, weight: "bold")
    set align(center)

    v(1em)
    grid(columns: (47%, 47%), gutter: 6%,
      {
        let todo-count = counter("todo").final().at(0);
        if (todo-count > 0) {
          set text(fill: red)
          block(width: 100%, inset: 4pt)[#todo-count TODOs remaining]
        }
      },
      {
        let note-count = counter("note").final().at(0);
        if (note-count > 0) {
          block(fill: yellow, width: 100%, inset: 4pt)[#note-count notes]
        }
      }
    )
  }
}


#let introduction(print, ..args) = {
  // hide empty pages from web version
  if print {
    // assignment must be on a single sheet from both sides
    pagebreak(to: "odd")
  } else {
    // Typst cannot embed PDFs, add the assignment separately
    //page[assignment page 1]
    //page[assignment page 2]
  }

  if print {
    pagebreak(to: "odd", weak: true)
  }
  abstract-page(..args)

  if print {
    // outline should be on the right, but the outline title has a pagebreak
    pagebreak(to: "even")
  }
  outline(
    depth: 3,
    title: [Obsah]
)

  pagebreak(weak: true)
}