
from .Qt import QtWidgets
from .itemStyle import ItemStyle
from .color import Color

from string import Formatter
from os import path


class MtoAStyle(object):

    __current_style = None

    @staticmethod
    def registerCurrentStyle(style):
        MtoAStyle.__currentStyle = style

    @staticmethod
    def currentStyle():
        return MtoAStyle.__current_style or MtoADarkStyle

    @classmethod
    def replaceColors(cls, styleobject, stylesheet):
        styleSheetString = ""
        for line in stylesheet:
            keys = {}
            for i in Formatter().parse(line):
                if i[1]:
                    keys[i[1]] = str(getattr(styleobject, i[1], ""))
            styleSheetString += line.format(**keys)
        return styleSheetString

    @classmethod
    def apply(cls, widget):
        raise NotImplementedError("MtoAStyle.apply()")

    @classmethod
    def applyImpl(cls, styleobject, widget):

        # style = QtWidgets.QStyleFactory.create("CleanLooks")
        style = ItemStyle(widget.style())
        stylesheet = file(styleobject.FILE_PATH, "r")

        style_string = cls.replaceColors(styleobject, stylesheet)

        setattr(widget, "__style_object__", style)
        widget.setStyle(getattr(widget, "__style_object__"))
        widget.setStyleSheet(style_string)


class MtoADarkStyle(MtoAStyle):

    FILE_PATH = path.join(path.split(path.realpath(__file__))[0], "qss", "style.qss")

    BACKGROUND_COLOR1 = Color(21, 21, 21)
    BACKGROUND_COLOR2 = Color(36, 36, 36)
    BACKGROUND_COLOR3 = Color(91, 91, 91)
    BACKGROUND_COLOR4 = Color(44, 44, 44)

    SELECTION_COLOR1 = Color(0, 0, 0)

    GRADIENT_COLOR1 = (Color(5, 5, 5), Color(61, 61, 61))

    TEXT_COLOR1 = Color(182, 182, 182)
    TEXT_COLOR2 = Color(182, 182, 182)

    BACKGROUND_SELECTED_COLOR1 = Color(220, 220, 220)
    BACKGROUND_SELECTED_COLOR2 = Color(220, 220, 220)

    BACKGROUND_COLOR_ALTERNATE1 = Color(55, 55, 55)

    BORDER_COLOR1 = Color(41, 41, 41)
    BORDER_COLOR2 = Color(65, 65, 65)
    BORDER_COLOR3 = Color(110, 110, 110)
    BORDER_COLOR4 = Color(0, 0, 0)

    @classmethod
    def apply(cls, widget):
        cls.applyImpl(MtoADarkStyle, widget)
