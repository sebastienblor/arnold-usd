from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .itemStyle import ItemStyle
from .utils import dpiScale

from arnold import *


DEFAULT_VISBILITIES = {'AI_RAY_CAMERA': True,
                       'AI_RAY_SHADOW': True,
                       'AI_RAY_DIFFUSE_REFLECT': True,
                       'AI_RAY_SPECULAR_REFLECT': True,
                       'AI_RAY_DIFFUSE_TRANSMIT': True,
                       'AI_RAY_SPECULAR_TRANSMIT': True,
                       'AI_RAY_VOLUME': True,
                       'AI_RAY_SUBSURFACE': True}

RAYS = [("ai_ray_camera", AI_RAY_CAMERA),
        ("ai_ray_shadow", AI_RAY_SHADOW),
        ("ai_ray_diffuse_transmit", AI_RAY_DIFFUSE_TRANSMIT),
        ("ai_ray_specular_transmit", AI_RAY_SPECULAR_TRANSMIT),
        ("ai_ray_volume", AI_RAY_VOLUME),
        ("ai_ray_diffuse_reflect", AI_RAY_DIFFUSE_REFLECT),
        ("ai_ray_specular_reflect", AI_RAY_SPECULAR_REFLECT),
        ("ai_ray_subsurface", AI_RAY_SUBSURFACE)]


def getVisibilityValue(ai_ray_camera=True,
                       ai_ray_shadow=True,
                       ai_ray_diffuse_transmit=True,
                       ai_ray_specular_transmit=True,
                       ai_ray_volume=True,
                       ai_ray_diffuse_reflect=True,
                       ai_ray_specular_reflect=True,
                       ai_ray_subsurface=True):

    vis = AI_RAY_ALL

    if not ai_ray_camera:
        vis &= ~AI_RAY_CAMERA
    if not ai_ray_shadow:
        vis &= ~AI_RAY_SHADOW
    if not ai_ray_diffuse_transmit:
        vis &= ~AI_RAY_DIFFUSE_TRANSMIT
    if not ai_ray_specular_transmit:
        vis &= ~AI_RAY_SPECULAR_TRANSMIT
    if not ai_ray_volume:
        vis &= ~AI_RAY_VOLUME
    if not ai_ray_diffuse_reflect:
        vis &= ~AI_RAY_DIFFUSE_REFLECT
    if not ai_ray_specular_reflect:
        vis &= ~AI_RAY_SPECULAR_REFLECT
    if not ai_ray_subsurface:
        vis &= ~AI_RAY_SUBSURFACE

    return vis


def getVisibilityDict(value):
    vis_dict = {}
    for n, v in RAYS:
        vis_dict[v] = True
    compViz = AI_RAY_ALL
    if value < compViz:
        for v in reversed([v for n, v in RAYS]):
            compViz &= ~v
            if value <= compViz:
                vis_dict[v] = False
            else:
                compViz += v
    return vis_dict


class MtoAVisibilityCheckBox(QtWidgets.QCheckBox):
    """docstring for MtoAVisibilityCheckBox"""
    def __init__(self, arg):
        super(MtoAVisibilityCheckBox, self).__init__()
        self.visibility = None


class MtoAVisibilityWidget(QtWidgets.QFrame):
    """docstring for MtoAVisibilityWidget"""

    valueChanged = QtCore.Signal(str, str, str, int)
    deleteAttr = QtCore.Signal(str, str, str)

    def __init__(self, title='test', parent=None):
        super(MtoAVisibilityWidget, self).__init__(parent)
        self.setLayout(QtWidgets.QVBoxLayout())
        self.label_frame = QtWidgets.QFrame()
        self.label_frame.setStyleSheet("QFrame {border:0px;background-color: rgba(0,0,0,0);}")
        self.label_frame.setLayout(QtWidgets.QHBoxLayout())
        self.label_frame.layout().setContentsMargins(0, 0, 0, 0)
        self.label = QtWidgets.QLabel(title)
        self.label_frame.layout().addWidget(self.label)

        self.removeWidget = QtWidgets.QPushButton()
        BBWidget.setStaticSize(self.removeWidget, 15, 15)
        self.removeWidget.setFlat(True)
        self.removeWidget.setStyleSheet(
            "QPushButton:flat {border: 0px solid rgb(0,0,0,0);background-color: rgba(0,0,0,0);}")
        self.removeWidget.setIcon(
            QIcon(self.style().standardPixmap(QStyle.SP_DockWidgetCloseButton)))
        self.removeWidget.setToolTip("Remove attribute from item")
        self.removeWidget.clicked.connect(self.emitDelete)
        self.label_frame.layout().addWidget(self.removeWidget)

        self.layout().addWidget(self.label_frame)

        self.node = None
        self.pattern = None
        self.attr = None

        self.vis_boxes = []
        # make a box per visibility option
        for name, vis in RAYS:
            _visBox = MtoAVisibilityCheckBox(' '.join(name.split('_')[2:]).title())
            _visBox.setChecked(True)
            _visBox.visibility = vis
            self.layout().addWidget(_visBox)
            self.vis_boxes.append(_visBox)
            _visBox.stateChanged.connect(self.emitValue)

    def getValue(self):

        vis = AI_RAY_ALL
        for cb in self.vis_boxes:
            if not cb.isChecked():
                vis &= ~cb.visibility
        return vis

    def setValue(self, value):
        vis_dict = getVisibilityDict(value)
        for vis_box in self.vis_boxes:
            self.visBoxGlossy.setChecked(vis_dict[vis_box.visibility])

    def emitValue(self, *args):
        self.valueChanged.emit(self.node,
                               self.pattern,
                               self.attr,
                               self.getValue())

    def emitDelete(self, *args):
        self.deleteAttr.emit(self.node, self.pattern, self.attr)