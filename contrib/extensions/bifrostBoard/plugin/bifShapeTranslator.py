import mtoa.ui.ae.templates as templates


class bifShapeDescriptionTemplate(templates.ShapeTranslatorTemplate):

    def setup(self):

        self.commonShapeAttributes()
        self.beginLayout("Translator Options", collapse=False)
        self.addControl("aiFilename", label="Bifrost JSON file", annotation='File path to the JSON file')
        self.addControl("aiCompound", label="Compound", annotation='Compund name in JSON file')
        self.addSeparator()
        self.addControl("namespace", label="Namespace", annotation='Procedural namespace')
        self.addSeparator()


templates.registerTranslatorUI(bifShapeDescriptionTemplate, "bifShape")
