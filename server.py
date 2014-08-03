from time import sleep
import tornado
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop
from tornado.web import Application, asynchronous, RequestHandler
from multiprocessing.pool import ThreadPool
import os
import urllib
import time
from tornado_cors import CorsMixin

import logging

logging.basicConfig(level=logging.DEBUG,filename="py.log")
 
_workers = ThreadPool(10)
 
def run_background(func, callback, args=(), kwds={}):
    def _callback(result):
        IOLoop.instance().add_callback(lambda: callback(result))
    _workers.apply_async(func, args, kwds, _callback)
 
# blocking task like querying to MySQL
def blocking_task(url, picType):

    if (picType == "icon"):
        png = url
    else:
        png = url + '/convert?format=png&w=500'

    # This code is all super hacky
    opener=urllib.URLopener()
    opener.retrieve(png, "face.png")

    print "a"

    if (picType == "icon"):
        os.system("convert face.png -gravity center -extent 400x400 face2.png")

    else:
        # Clean it up
        os.system("convert face.png -sharpen 0x2 +dither -colors 3 -colorspace gray -normalize -despeckle -noise 5 face2.png")

    print "c"
    print picType

    # Make it into an OBJ
    os.system("/app/anneal face2.png btc.png facecoin.obj")

    print "d"

    # Store
    filename = str(time.time()) + ".obj"

    os.system("mv facecoin.obj obj/%s" % str(filename))

    return filename;
 
class Handler(CorsMixin, RequestHandler):
    CORS_ORIGIN = '*'
    
    @asynchronous
    def get(self):
    	url = self.get_argument("url", None, "")
        pType = self.get_argument("type", None, "photo")

        run_background(blocking_task, self.on_complete, (url, pType))
 
    def on_complete(self, res):
        self.write(res);
        self.finish()

class StaticHandler(CorsMixin, tornado.web.StaticFileHandler):
    CORS_ORIGIN = '*'

settings = dict(
    static_path=os.path.join(os.path.dirname(__file__), "obj"),
    debug=True
)

handlers = [("/", Handler), (r"/(.*)", StaticHandler, {"path": "./obj"})]

application = Application(handlers, **settings)

HTTPServer(application).listen(5000)
IOLoop.instance().start()