from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .itemStyle import ItemStyle
from .utils import dpiScale
from .style import MtoAStyle
from .syntaxhighlighter import CppHighlighter


class LineNumberArea(QtWidgets.QWidget):
    """LineNumberArea widget for drawing the line number"""
    def __init__(self, editor):
        super(LineNumberArea, self).__init__(parent=editor)
        self.codeEditor = editor

    def sizeHint(self):
        return QtWidgets.QSize(self.codeEditor.lineNumberAreaWidth(), 0)

    # protected
    def paintEvent(self, event):
        self.codeEditor.lineNumberAreaPaintEvent(event)


class BaseCodeEditor(QtWidgets.QPlainTextEdit):
    """Base coed editor box"""

    def __init__(self, parent=None, syntax_higlighter=None, style=None):
        super(BaseCodeEditor, self).__init__(parent)

        self.lineNumberArea = LineNumberArea(self)

        if not style:
            style = MtoAStyle.currentStyle()
        style.apply(self)

        font = QtGui.QFont()
        font.setFamily("Courier")
        font.setFixedPitch(True)
        font.setPointSize(8)
        self.setFont(font)

        self.highlighter = None
        if syntax_higlighter:
            self.highlighter = syntax_higlighter(self.document())

        self.blockCountChanged[int].connect(self.updateLineNumberAreaWidth)
        self.updateRequest.connect(self.updateLineNumberArea)
        self.cursorPositionChanged.connect(self.highlightCurrentLine)

        self.updateLineNumberAreaWidth(0)
        self.highlightCurrentLine()

    def lineNumberAreaWidth(self):
        digits = 4
        space = 3 + self.fontMetrics().width('9') * digits

        return space

    def resizeEvent(self, event):
        super(BaseCodeEditor, self).resizeEvent(event)
        cr = self.contentsRect()
        self.lineNumberArea.setGeometry(QtCore.QRect(cr.left(), cr.top(),
                                        self.lineNumberAreaWidth(), cr.height()))

    @QtCore.Slot(int)
    def updateLineNumberAreaWidth(self, newBlockCount):
        self.setViewportMargins(self.lineNumberAreaWidth(), 0, 0, 0)

    @QtCore.Slot(QtCore.QRect, int)
    def updateLineNumberArea(self, rect, dy):
        if dy:
            self.lineNumberArea.scroll(0, dy)
        else:
            self.lineNumberArea.update(0, rect.y(), self.lineNumberArea.width(), rect.height())

        if rect.contains(self.viewport().rect()):
            self.updateLineNumberAreaWidth(0)

    @QtCore.Slot()
    def highlightCurrentLine(self):

        extraSelections = []

        if not self.isReadOnly():
            selection = QtWidgets.QTextEdit.ExtraSelection()

            lineColor = QtGui.QColor(90, 90, 90)

            selection.format.setBackground(lineColor)
            selection.format.setProperty(QtGui.QTextFormat.FullWidthSelection, True)
            selection.cursor = self.textCursor()
            selection.cursor.clearSelection()
            extraSelections.append(selection)

        self.setExtraSelections(extraSelections)

    def lineNumberAreaPaintEvent(self, event):
        painter = QtGui.QPainter(self.lineNumberArea)
        painter.fillRect(event.rect(), QtCore.Qt.lightGray)
        font = self.font()
        painter.setFont(font)

        block = self.firstVisibleBlock()
        blockNumber = block.blockNumber()
        top = self.blockBoundingGeometry(block).translated(self.contentOffset()).top()
        bottom = top + self.blockBoundingRect(block).height()
        while block.isValid() and top <= event.rect().bottom():
            if block.isVisible() and bottom >= event.rect().top():
                number = str(blockNumber + 1)
                painter.setPen(QtCore.Qt.black)
                painter.drawText(0, top, self.lineNumberArea.width(), self.fontMetrics().height(),
                                 QtCore.Qt.AlignCenter, number)

            block = next(block)
            top = bottom
            bottom = top + self.blockBoundingRect(block).height()
            blockNumber += 1
