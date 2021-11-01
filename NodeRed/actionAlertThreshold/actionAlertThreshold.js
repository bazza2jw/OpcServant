module.exports = function (RED) {
    function ActionAlertThreshold(config) {
        RED.nodes.createNode(this, config);
        var node = this;
        //
        // Locally store the scale and offset
        node.hihi = parseFloat(config.hihi);
        node.hilo = parseFloat(config.hilo);
        node.lohi = parseFloat(config.lohi);
        node.lolo = parseFloat(config.lolo);
        node.hihiEnable = config.hihiEnable;
        node.hiloEnable = config.hiloEnable;
        node.lohiEnable = config.lohiEnable;
        node.loloEnable = config.loloEnable;
        //
        //
        node.okonvalue =RED.util.evaluateNodeProperty(config.okonvalue,config.okonvalueType,node); // translate to correct type
        node.okoffvalue =RED.util.evaluateNodeProperty(config.okoffvalue,config.okoffvalueType,node); // translate to correct type
        node.alertonvalue =RED.util.evaluateNodeProperty(config.okalertvalue,config.alertonvalueType,node); // translate to correct type
        node.alertoffvalue =RED.util.evaluateNodeProperty(config.alertoffvalue,config.alertoffvalueType,node); // translate to correct type
        node.actiononvalue =RED.util.evaluateNodeProperty(config.actiononvalue,config.actiononvalueType,node); // translate to correct type
        node.actionoffvalue =RED.util.evaluateNodeProperty(config.actionoffvalue,config.actionoffvalueType,node); // translate to correct type
        //
        //    
        node.on('input', function (msg) {
            var v = 0.0;

            if (typeof (msg.payload) == 'number') {
                v = msg.payload;
            } else if (typeof (msg.payload) == 'string') {
                v = parseFloat(msg.payload);
            }

            var alert = false;
            var action = false;
            var ok = true;
            
            if( ( this.hihiEnable) && (v > this.hihi))
            {
                action = true;
            }
            else if( ( this.hiloEnable) && (v > this.hilo))
            {
                alert = true;
            }

            if( ( this.loloEnable) && (v < this.lolo))
            {
                action = true;
            }
            else if( ( this.lohiEnable) && (v < this.lohi))
            {
                alert = true;
            }

            ok = ((alert || action) == false);

            msg.payload = v;
            //  
            
            
            // we have four outputs  value, ok, action , alert
            var state = 'ok';
            if(action)
            {
                state = 'action';
            }
            else if(alert) {
                state = 'alert';
            }
            //
            // set flags - the messages may drive outputs - so we need on and off for each possible status
            //
            var okMsg = {payload:ok?this.okonvalue:this.okoffvalue, topic:msg.topic, value:v, state:state};
            var alertMsg = {payload:alert?this.alertonvalue:this.alertoffvalue, topic:msg.topic, value:v, state:state};
            var actionMsg = {payload:action?this.actiononvalue:this.actionoffvalue, topic:msg.topic, value:v, state:state};
            //
            if(action)
            {
                this.status({fill:"red",shape:"dot",text:"Action"});
            }
            else if(alert)
            {
                this.status({fill:"yellow",shape:"dot",text:"Alert"});
            }
            else
            {
                this.status({fill:"green",shape:"dot",text:"OK"});
            }
            //
            msg.state = state; // add the value's alarm state to the value message - use this to connect to alarm groups
            node.send([msg,okMsg,alertMsg,actionMsg]);
        });
    }
    RED.nodes.registerType("actionalertthreshold", ActionAlertThreshold);
}
