from .Qt import QtCore
from .Qt import QtGui
import os.path as path
import json


SYNTAX_DIR = path.join(path.split(path.realpath(__file__))[0], "syntax")


class BaseHighlighter(QtGui.QSyntaxHighlighter):

    def __init__(self, parent=None):
        super(BaseHighlighter, self).__init__(parent)

        self.json_data = {}
        self.highlightingRules = []
        self.commentStartExpression = QtCore.QRegExp("/\\*")
        self.commentEndExpression = QtCore.QRegExp("\\*/")
        self.multiLineCommentFormat = QtGui.QTextCharFormat()

    def loadSyntaxJSON(self, json_data):


        if json_data:
            self.json_data = json.load(json_data)

            # load syntax patterns
            for root in self.json_data:
                for key, data in self.json_data[root].items():
                    _format = QtGui.QTextCharFormat()
                    if data.get("weight") == "bold":
                        _format.setFontWeight(QtGui.QFont.Bold)
                    if data.get("italic"):
                        _format.setFontItalic(True)
                    _syntaxColor = data.get("color", [128, 128, 128])
                    _format.setForeground(QtGui.QColor(*_syntaxColor))
                    _patterns = data.get("patterns", [])
                    for pat in _patterns:
                        self.highlightingRules.append((QtCore.QRegExp(pat), _format))

                    if key == 'single-line' and root == 'comment':
                        _patterns = data.get("patterns", [])
                        for pat in _patterns:
                            self.highlightingRules.append((QtCore.QRegExp(pat), _format))
                    elif key == 'multi-line' and root == 'comment':
                        self.commentStartExpression = QtCore.QRegExp(str(data.get("start_pattern", "")))
                        self.commentEndExpression = QtCore.QRegExp(data.get("end_pattern", ""))
                        self.multiLineCommentFormat = _format

    def highlightBlock(self, text):
        for pattern, fmt in self.highlightingRules:
            expression = QtCore.QRegExp(pattern)
            index = expression.indexIn(text)
            while index >= 0:
                length = expression.matchedLength()
                self.setFormat(index, length, fmt)
                index = expression.indexIn(text, index + length)

        self.setCurrentBlockState(0)

        startIndex = 0
        if self.previousBlockState() != 1:
            startIndex = self.commentStartExpression.indexIn(text)

        while startIndex >= 0:
            endIndex = self.commentEndExpression.indexIn(text, startIndex)

            if endIndex == -1:
                self.setCurrentBlockState(1)
                commentLength = len(text) - startIndex
            else:
                commentLength = endIndex - startIndex + self.commentEndExpression.matchedLength()

            self.setFormat(startIndex, commentLength,
                    self.multiLineCommentFormat)
            startIndex = self.commentStartExpression.indexIn(text,
                    startIndex + commentLength)


class CppHighlighter(BaseHighlighter):

    FILE_PATH = path.join(SYNTAX_DIR, "cpp.json")

    def __init__(self, parent=None):
        super(CppHighlighter, self).__init__(parent)

        json_data = open(self.FILE_PATH)
        self.loadSyntaxJSON(json_data)
