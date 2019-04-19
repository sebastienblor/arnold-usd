import mtoa.ui.ae.templates as templates


class bifShapeDescriptionTemplate(templates.ShapeTranslatorTemplate):

    def setup(self):

        self.commonShapeAttributes()
        self.beginLayout("Translator Options", collapse=False)
        self.addControl("aiFilename", label="Bifrost JSON file", annotation='File path to the JSON file')
        self.addControl("aiCompound", label="Compound", annotation='Compound name in JSON file')
        self.addControl("aiFastBifHack", label="Fast BIF handover hack", annotation='Hand Bifrost data directly to Arnold, incompatible with writing to .ass files.')
        self.addSeparator()
        self.addControl("aiNamespace", label="Namespace", annotation='Procedural namespace')
        self.addControl("aiUserOptions", label="User Options", annotation='Custom User Options (advanced)')
        self.addSeparator()


templates.registerAETemplate(bifShapeDescriptionTemplate, "bifShape")
