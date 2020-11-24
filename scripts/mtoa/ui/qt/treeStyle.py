

from .Qt import QtWidgets, QtCore, QtGui

from .utils import dpiScale


class TreeStyle(QtWidgets.QCommonStyle):
    """
    This class defines the RenderSetup style and is only used when style sheets and the delegate are not sufficient.
    """

    # Constants
    DROP_INDICATOR_COLOR = QtGui.QColor(255, 255, 255)
    DROP_INDICATOR_WIDTH = dpiScale(3)
    DROP_INDICATOR_LEFT_OFFSET = dpiScale(-25)

    def __init__(self):
        super(TreeStyle, self).__init__()

    def drawComplexControl(self, control, option, painter, widget = None):
        return QtWidgets.QApplication.style().drawComplexControl(control, option, painter, widget)

    def drawControl(self, element, option, painter, widget = None):
        return QtWidgets.QApplication.style().drawControl(element, option, painter, widget)

    def drawItemPixmap(self, painter, rectangle, alignment, pixmap):
        return QtWidgets.QApplication.style().drawItemPixmap(painter, rectangle, alignment, pixmap)

    def drawItemText(self, painter, rectangle, alignment, palette, enabled, text, textRole = QtGui.QPalette.NoRole):
        return QtWidgets.QApplication.style().drawItemText(painter, rectangle, alignment, palette, enabled, text, textRole)

    def drawPrimitive(self, element, option, painter, widget = None):
        """ Draws the given primitive element with the provided painter using the style options specified by option. """
        
        # Changes the way the drop indicator is drawn
        if element == QtWidgets.QStyle.PE_IndicatorItemViewItemDrop and not option.rect.isNull():
            painter.save()
            painter.setRenderHint(QtGui.QPainter.Antialiasing, True)
            oldPen = painter.pen()
            painter.setPen(QtGui.QPen(self.DROP_INDICATOR_COLOR, self.DROP_INDICATOR_WIDTH))
            rect = option.rect
            rect.setLeft(rect.left() + self.DROP_INDICATOR_LEFT_OFFSET)
            if option.rect.height() == 0:
                painter.drawLine(rect.topLeft(), option.rect.topRight())
            else:
                painter.drawRect(rect)
            painter.setPen(oldPen)
            painter.restore()

    def generatedIconPixmap(self, iconMode, pixmap, option):
        return QtWidgets.QApplication.style().generatedIconPixmap(iconMode, pixmap, option)

    def hitTestComplexControl(self, control, option, position, widget = None):
        return QtWidgets.QApplication.style().hitTestComplexControl(control, option, position, widget)

    def itemPixmapRect(self, rectangle, alignment, pixmap):
        return QtWidgets.QApplication.style().itemPixmapRect(rectangle, alignment, pixmap)

    def itemTextRect(self, metrics, rectangle, alignment, enabled, text):
        return QtWidgets.QApplication.style().itemTextRect(metrics, rectangle, alignment, enabled, text)

    def pixelMetric(self, metric, option = None, widget = None):
        return QtWidgets.QApplication.style().pixelMetric(metric, option, widget)

    def polish(self, *args, **kwargs):
        return QtWidgets.QApplication.style().polish(*args, **kwargs)

    def styleHint(self, hint, option=None, widget=None, returnData=None):
        if hint == QtWidgets.QStyle.SH_Slider_AbsoluteSetButtons:
            return QtCore.Qt.LeftButton | QtCore.Qt.MidButton | QtCore.Qt.RightButton
        return QtWidgets.QApplication.style().styleHint(hint, option, widget, returnData)

    def subControlRect(self, control, option, subControl, widget = None):
        return QtWidgets.QApplication.style().subControlRect(control, option, subControl, widget)

    def subElementRect(self, element, option, widget = None):
        return QtWidgets.QApplication.style().subElementRect(element, option, widget)

    def unpolish(self, *args, **kwargs):
        return QtWidgets.QApplication.style().unpolish(*args, **kwargs)

    def sizeFromContents(self, ct, opt, contentsSize, widget = None):
        return QtWidgets.QApplication.style().sizeFromContents(ct, opt, contentsSize, widget)
