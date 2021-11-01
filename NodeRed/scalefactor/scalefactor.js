module.exports = function (RED) {
    function ScaleFactor(config) {
        RED.nodes.createNode(this, config);
        var node = this;
        // Locally store the scale and offset
        node.scale = parseFloat(config.scale);
        node.offset = parseFloat(config.offset);
        node.offsetThenScale = config.offsetThenScale;
        node.topic = config.topic;
        //    
        node.on('input', function (msg) {
            var v = 0.0;
            // MQTT messages from OpcServant are objects with the Value  property
            if ((typeof(msg.payload) == 'object') && ('Value' in msg.payload)) {
                if (typeof (msg.payload.Value) == 'number') {
                    v = msg.payload.Value;
                } else if (typeof (msg.payload.Value) == 'string') {
                    v = parseFloat(msg.payload.Value);
                }
            }
            else {
                if (typeof (msg.payload) == 'number') {
                    v = msg.payload;
                } else if (typeof (msg.payload) == 'string') {
                    v = parseFloat(msg.payload);
                }
            }

            if (this.offsetThenScale) {
                v = (v + this.offset) * this.scale;

            } else {
                v = v * this.scale + this.offset;
            }

            msg.payload = v;
            if (node.topic.length > 0) {
                msg.topic = node.topic; // translate the topic
            }
            node.send(msg);
        });
    }
    RED.nodes.registerType("scalefactor", ScaleFactor);
}
