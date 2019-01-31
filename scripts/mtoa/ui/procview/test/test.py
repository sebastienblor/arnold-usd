
import sys
import os
__path = os.path.abspath(os.path.dirname(__file__)+'/../../../../../../scripts')
print __path
sys.path.insert(0, __path)


class TestAbcViewWindow(BaseWindow):

    def __init__(self, parent=None):
        super(TestAbcViewWindow, self).__init__(parent)

        self.tree = AbcTreeView(transverser=AlembicTransverser(), parent=self)

        self.setCentralWidget(self.tree)


if __name__ == '__main__':

    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = TestAbcViewWindow()
    window.resize(640, 512)
    window.show()
    sys.exit(app.exec_())





if __name__ == '__main__':

    impl = AlembicTransverserImpl()
    iarch = Abc.IArchive(str("/data/Tutorials/alembic_pipe/basic_scene.abc"))
    abc_items = impl.visitObject(iarch.getTop())

    path = '/'

    children = []
    for obj in abc_items:
        if obj[PROC_PATH] == path:
            iobj = obj[PROC_IOBJECT]
            for i in abc_items:
                if i[PROC_PARENT] == path:
                    children.append(i)
            break
    for ch in children:
        print ch
