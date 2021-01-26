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
        self.singleLineCommentExpression = QtCore.QRegExp("//[^\n]*")
        self.commentStartExpression = QtCore.QRegExp("/\\*")
        self.commentEndExpression = QtCore.QRegExp("\\*/")
        self.multiLineCommentFormat = QtGui.QTextCharFormat()
        self.metadataStartExpression = QtCore.QRegExp("/\\*")
        self.metadataEndExpression = QtCore.QRegExp("\\*/")
        self.multiLineMetadataFormat = QtGui.QTextCharFormat()
        self.errorColor = QtGui.QColor(255, 255, 0)

    def loadSyntaxJSON(self, json_data):
        if json_data:
            self.json_data = json.load(json_data)

            # load syntax patterns
            for root in self.json_data:

                for key, data in self.json_data[root].items():
                    if root in ["comments", "metadata"]:
                        continue
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

            for k in ["comments", "metadata"]:
                if k in self.json_data:
                    for key, data in self.json_data[k].items():
                        _format = QtGui.QTextCharFormat()
                        if data.get("weight") == "bold":
                            _format.setFontWeight(QtGui.QFont.Bold)
                        if data.get("italic"):
                            _format.setFontItalic(True)
                        _syntaxColor = data.get("color", [128, 128, 128])
                        _format.setForeground(QtGui.QColor(*_syntaxColor))

                        if key == 'single-line' and k == 'comments':
                            _patterns = data.get("patterns", [])
                            for pat in _patterns:
                                regex = QtCore.QRegExp(pat)
                                self.highlightingRules.append((regex, _format))
                                self.singleLineCommentExpression = regex
                        elif key == 'multi-line' and k == 'comments':
                            self.commentStartExpression = QtCore.QRegExp(str(data.get("start_pattern", "")))
                            self.commentEndExpression = QtCore.QRegExp(data.get("end_pattern", ""))
                            self.multiLineCommentFormat = _format
                        elif key == 'multi-line' and k == 'metadata':
                            self.metadataStartExpression = QtCore.QRegExp(str(data.get("start_pattern", "")))
                            self.metadataEndExpression = QtCore.QRegExp(data.get("end_pattern", ""))
                            self.multiLineMetadataFormat = _format

    def highlightBlock(self, text):
        for pattern, fmt in self.highlightingRules:
            index = pattern.indexIn(text)
            while index >= 0:
                length = pattern.matchedLength()
                self.setFormat(index, length, fmt)
                index = pattern.indexIn(text, index + length)

        self.setCurrentBlockState(0)

        startIndex = 0
        if self.previousBlockState() != 1:
            startIndex = self.commentStartExpression.indexIn(text)

            # Single line comments before a multi-line comments don't start a multi-line comment
            singleIndex = self.singleLineCommentExpression.indexIn(text, startIndex)
            if singleIndex >= 0 and singleIndex < startIndex:
                startIndex = -1

        while startIndex >= 0:
            endIndex = self.commentEndExpression.indexIn(text, startIndex)

            if endIndex == -1:
                self.setCurrentBlockState(1)
                commentLength = len(text) - startIndex
            else:
                commentLength = endIndex - startIndex + self.commentEndExpression.matchedLength()

            self.setFormat(startIndex, commentLength, self.multiLineCommentFormat)
            startIndex = self.commentStartExpression.indexIn(text, startIndex + commentLength)

        startIndex = 0
        if self.previousBlockState() != 2:
            startIndex = self.metadataStartExpression.indexIn(text)

            # Single line comments before a multi-line comments don't start a multi-line comment
            singleIndex = self.singleLineCommentExpression.indexIn(text, startIndex)
            if singleIndex >= 0 and singleIndex < startIndex:
                startIndex = -1

            # Also multi-line comments starting, OR being INSIDE one...
            multiIndex = self.commentStartExpression.indexIn(text, startIndex)
            if multiIndex >= 0 and multiIndex < startIndex  or self.previousBlockState() == 1:
                startIndex = -1


        while startIndex >= 0:
            endIndex = self.metadataEndExpression.indexIn(text, startIndex)

            if endIndex == -1:
                self.setCurrentBlockState(2)
                metadataLength = len(text) - startIndex
            else:
                metadataLength = endIndex - startIndex + self.metadataEndExpression.matchedLength()

            self.setFormat(startIndex, metadataLength, self.multiLineMetadataFormat)
            startIndex = self.metadataStartExpression.indexIn(text, startIndex + metadataLength)


class CppHighlighter(BaseHighlighter):

    FILE_PATH = path.join(SYNTAX_DIR, "cpp.json")

    def __init__(self, parent=None):
        super(CppHighlighter, self).__init__(parent)

        json_data = open(self.FILE_PATH)
        self.loadSyntaxJSON(json_data)


class OSLHighlighter(BaseHighlighter):

    FILE_PATH = path.join(SYNTAX_DIR, "osl.json")

    def __init__(self, parent=None):
        super(OSLHighlighter, self).__init__(parent)

        json_data = open(self.FILE_PATH)
        self.loadSyntaxJSON(json_data)
