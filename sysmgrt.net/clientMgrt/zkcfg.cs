﻿//------------------------------------------------------------------------------
// <auto-generated>
//     此代码由工具生成。
//     运行时版本:4.0.30319.34003
//
//     对此文件的更改可能会导致不正确的行为，并且如果
//     重新生成代码，这些更改将会丢失。
// </auto-generated>
//------------------------------------------------------------------------------

// 
// 此源代码由 wsdl 自动生成, Version=4.0.30319.1。
// 
namespace zkconfig {
    using System.Diagnostics;
    using System;
    using System.Xml.Serialization;
    using System.ComponentModel;
    using System.Web.Services.Protocols;
    using System.Web.Services;
    
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Web.Services.WebServiceBindingAttribute(Name="zkcfg", Namespace="http://www.zonekey.com/sysmgrt/config")]
    public partial class zkcfg : System.Web.Services.Protocols.SoapHttpClientProtocol {
        
        private System.Threading.SendOrPostCallback getAllKeysOperationCompleted;
        
        private System.Threading.SendOrPostCallback getValueOperationCompleted;
        
        private System.Threading.SendOrPostCallback setValueOperationCompleted;
        
        private System.Threading.SendOrPostCallback delKeyOperationCompleted;
        
        /// <remarks/>
        public zkcfg() {
            this.Url = "http://localhost:80";
        }
        
        /// <remarks/>
        public event getAllKeysCompletedEventHandler getAllKeysCompleted;
        
        /// <remarks/>
        public event getValueCompletedEventHandler getValueCompleted;
        
        /// <remarks/>
        public event setValueCompletedEventHandler setValueCompleted;
        
        /// <remarks/>
        public event delKeyCompletedEventHandler delKeyCompleted;
        
        /// <remarks/>
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Bare)]
        [return: System.Xml.Serialization.XmlElementAttribute("getAllKeysResponse", Namespace="http://www.zonekey.com/sysmgrt/config")]
        public getAllKeysResponse getAllKeys([System.Xml.Serialization.XmlElementAttribute("getAllKeys", Namespace="http://www.zonekey.com/sysmgrt/config")] getAllKeys getAllKeys1) {
            object[] results = this.Invoke("getAllKeys", new object[] {
                        getAllKeys1});
            return ((getAllKeysResponse)(results[0]));
        }
        
        /// <remarks/>
        public System.IAsyncResult BegingetAllKeys(getAllKeys getAllKeys1, System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("getAllKeys", new object[] {
                        getAllKeys1}, callback, asyncState);
        }
        
        /// <remarks/>
        public getAllKeysResponse EndgetAllKeys(System.IAsyncResult asyncResult) {
            object[] results = this.EndInvoke(asyncResult);
            return ((getAllKeysResponse)(results[0]));
        }
        
        /// <remarks/>
        public void getAllKeysAsync(getAllKeys getAllKeys1) {
            this.getAllKeysAsync(getAllKeys1, null);
        }
        
        /// <remarks/>
        public void getAllKeysAsync(getAllKeys getAllKeys1, object userState) {
            if ((this.getAllKeysOperationCompleted == null)) {
                this.getAllKeysOperationCompleted = new System.Threading.SendOrPostCallback(this.OngetAllKeysOperationCompleted);
            }
            this.InvokeAsync("getAllKeys", new object[] {
                        getAllKeys1}, this.getAllKeysOperationCompleted, userState);
        }
        
        private void OngetAllKeysOperationCompleted(object arg) {
            if ((this.getAllKeysCompleted != null)) {
                System.Web.Services.Protocols.InvokeCompletedEventArgs invokeArgs = ((System.Web.Services.Protocols.InvokeCompletedEventArgs)(arg));
                this.getAllKeysCompleted(this, new getAllKeysCompletedEventArgs(invokeArgs.Results, invokeArgs.Error, invokeArgs.Cancelled, invokeArgs.UserState));
            }
        }
        
        /// <remarks/>
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Bare)]
        [return: System.Xml.Serialization.XmlElementAttribute("RetRes", Namespace="http://www.zonekey.com/sysmgrt/config")]
        public Ret getValue([System.Xml.Serialization.XmlElementAttribute("getValue", Namespace="http://www.zonekey.com/sysmgrt/config")] getValue getValue1) {
            object[] results = this.Invoke("getValue", new object[] {
                        getValue1});
            return ((Ret)(results[0]));
        }
        
        /// <remarks/>
        public System.IAsyncResult BegingetValue(getValue getValue1, System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("getValue", new object[] {
                        getValue1}, callback, asyncState);
        }
        
        /// <remarks/>
        public Ret EndgetValue(System.IAsyncResult asyncResult) {
            object[] results = this.EndInvoke(asyncResult);
            return ((Ret)(results[0]));
        }
        
        /// <remarks/>
        public void getValueAsync(getValue getValue1) {
            this.getValueAsync(getValue1, null);
        }
        
        /// <remarks/>
        public void getValueAsync(getValue getValue1, object userState) {
            if ((this.getValueOperationCompleted == null)) {
                this.getValueOperationCompleted = new System.Threading.SendOrPostCallback(this.OngetValueOperationCompleted);
            }
            this.InvokeAsync("getValue", new object[] {
                        getValue1}, this.getValueOperationCompleted, userState);
        }
        
        private void OngetValueOperationCompleted(object arg) {
            if ((this.getValueCompleted != null)) {
                System.Web.Services.Protocols.InvokeCompletedEventArgs invokeArgs = ((System.Web.Services.Protocols.InvokeCompletedEventArgs)(arg));
                this.getValueCompleted(this, new getValueCompletedEventArgs(invokeArgs.Results, invokeArgs.Error, invokeArgs.Cancelled, invokeArgs.UserState));
            }
        }
        
        /// <remarks/>
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Bare)]
        [return: System.Xml.Serialization.XmlElementAttribute("RetRes", Namespace="http://www.zonekey.com/sysmgrt/config")]
        public Ret setValue([System.Xml.Serialization.XmlElementAttribute("setValue", Namespace="http://www.zonekey.com/sysmgrt/config")] setValue setValue1) {
            object[] results = this.Invoke("setValue", new object[] {
                        setValue1});
            return ((Ret)(results[0]));
        }
        
        /// <remarks/>
        public System.IAsyncResult BeginsetValue(setValue setValue1, System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("setValue", new object[] {
                        setValue1}, callback, asyncState);
        }
        
        /// <remarks/>
        public Ret EndsetValue(System.IAsyncResult asyncResult) {
            object[] results = this.EndInvoke(asyncResult);
            return ((Ret)(results[0]));
        }
        
        /// <remarks/>
        public void setValueAsync(setValue setValue1) {
            this.setValueAsync(setValue1, null);
        }
        
        /// <remarks/>
        public void setValueAsync(setValue setValue1, object userState) {
            if ((this.setValueOperationCompleted == null)) {
                this.setValueOperationCompleted = new System.Threading.SendOrPostCallback(this.OnsetValueOperationCompleted);
            }
            this.InvokeAsync("setValue", new object[] {
                        setValue1}, this.setValueOperationCompleted, userState);
        }
        
        private void OnsetValueOperationCompleted(object arg) {
            if ((this.setValueCompleted != null)) {
                System.Web.Services.Protocols.InvokeCompletedEventArgs invokeArgs = ((System.Web.Services.Protocols.InvokeCompletedEventArgs)(arg));
                this.setValueCompleted(this, new setValueCompletedEventArgs(invokeArgs.Results, invokeArgs.Error, invokeArgs.Cancelled, invokeArgs.UserState));
            }
        }
        
        /// <remarks/>
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Bare)]
        [return: System.Xml.Serialization.XmlElementAttribute("RetRes", Namespace="http://www.zonekey.com/sysmgrt/config")]
        public Ret delKey([System.Xml.Serialization.XmlElementAttribute("delKey", Namespace="http://www.zonekey.com/sysmgrt/config")] delKey delKey1) {
            object[] results = this.Invoke("delKey", new object[] {
                        delKey1});
            return ((Ret)(results[0]));
        }
        
        /// <remarks/>
        public System.IAsyncResult BegindelKey(delKey delKey1, System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("delKey", new object[] {
                        delKey1}, callback, asyncState);
        }
        
        /// <remarks/>
        public Ret EnddelKey(System.IAsyncResult asyncResult) {
            object[] results = this.EndInvoke(asyncResult);
            return ((Ret)(results[0]));
        }
        
        /// <remarks/>
        public void delKeyAsync(delKey delKey1) {
            this.delKeyAsync(delKey1, null);
        }
        
        /// <remarks/>
        public void delKeyAsync(delKey delKey1, object userState) {
            if ((this.delKeyOperationCompleted == null)) {
                this.delKeyOperationCompleted = new System.Threading.SendOrPostCallback(this.OndelKeyOperationCompleted);
            }
            this.InvokeAsync("delKey", new object[] {
                        delKey1}, this.delKeyOperationCompleted, userState);
        }
        
        private void OndelKeyOperationCompleted(object arg) {
            if ((this.delKeyCompleted != null)) {
                System.Web.Services.Protocols.InvokeCompletedEventArgs invokeArgs = ((System.Web.Services.Protocols.InvokeCompletedEventArgs)(arg));
                this.delKeyCompleted(this, new delKeyCompletedEventArgs(invokeArgs.Results, invokeArgs.Error, invokeArgs.Cancelled, invokeArgs.UserState));
            }
        }
        
        /// <remarks/>
        public new void CancelAsync(object userState) {
            base.CancelAsync(userState);
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.zonekey.com/sysmgrt/config")]
    public partial class getAllKeys {
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(Namespace="http://www.zonekey.com/sysmgrt/config")]
    public partial class Ret {
        
        private int resultField;
        
        private string valueField;
        
        /// <remarks/>
        public int result {
            get {
                return this.resultField;
            }
            set {
                this.resultField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=true)]
        public string value {
            get {
                return this.valueField;
            }
            set {
                this.valueField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.zonekey.com/sysmgrt/config")]
    public partial class getAllKeysResponse {
        
        private string[] keysResField;
        
        /// <remarks/>
        [System.Xml.Serialization.XmlArrayItemAttribute("item")]
        public string[] KeysRes {
            get {
                return this.keysResField;
            }
            set {
                this.keysResField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.zonekey.com/sysmgrt/config")]
    public partial class getValue {
        
        private string getValueKeyRequestField;
        
        /// <remarks/>
        public string getValueKeyRequest {
            get {
                return this.getValueKeyRequestField;
            }
            set {
                this.getValueKeyRequestField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.zonekey.com/sysmgrt/config")]
    public partial class setValue {
        
        private string setValueKeyRequestField;
        
        private string valueField;
        
        /// <remarks/>
        public string setValueKeyRequest {
            get {
                return this.setValueKeyRequestField;
            }
            set {
                this.setValueKeyRequestField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=true)]
        public string value {
            get {
                return this.valueField;
            }
            set {
                this.valueField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.zonekey.com/sysmgrt/config")]
    public partial class delKey {
        
        private string delKeyRequestField;
        
        /// <remarks/>
        public string delKeyRequest {
            get {
                return this.delKeyRequestField;
            }
            set {
                this.delKeyRequestField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    public delegate void getAllKeysCompletedEventHandler(object sender, getAllKeysCompletedEventArgs e);
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    public partial class getAllKeysCompletedEventArgs : System.ComponentModel.AsyncCompletedEventArgs {
        
        private object[] results;
        
        internal getAllKeysCompletedEventArgs(object[] results, System.Exception exception, bool cancelled, object userState) : 
                base(exception, cancelled, userState) {
            this.results = results;
        }
        
        /// <remarks/>
        public getAllKeysResponse Result {
            get {
                this.RaiseExceptionIfNecessary();
                return ((getAllKeysResponse)(this.results[0]));
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    public delegate void getValueCompletedEventHandler(object sender, getValueCompletedEventArgs e);
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    public partial class getValueCompletedEventArgs : System.ComponentModel.AsyncCompletedEventArgs {
        
        private object[] results;
        
        internal getValueCompletedEventArgs(object[] results, System.Exception exception, bool cancelled, object userState) : 
                base(exception, cancelled, userState) {
            this.results = results;
        }
        
        /// <remarks/>
        public Ret Result {
            get {
                this.RaiseExceptionIfNecessary();
                return ((Ret)(this.results[0]));
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    public delegate void setValueCompletedEventHandler(object sender, setValueCompletedEventArgs e);
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    public partial class setValueCompletedEventArgs : System.ComponentModel.AsyncCompletedEventArgs {
        
        private object[] results;
        
        internal setValueCompletedEventArgs(object[] results, System.Exception exception, bool cancelled, object userState) : 
                base(exception, cancelled, userState) {
            this.results = results;
        }
        
        /// <remarks/>
        public Ret Result {
            get {
                this.RaiseExceptionIfNecessary();
                return ((Ret)(this.results[0]));
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    public delegate void delKeyCompletedEventHandler(object sender, delKeyCompletedEventArgs e);
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("wsdl", "4.0.30319.1")]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    public partial class delKeyCompletedEventArgs : System.ComponentModel.AsyncCompletedEventArgs {
        
        private object[] results;
        
        internal delKeyCompletedEventArgs(object[] results, System.Exception exception, bool cancelled, object userState) : 
                base(exception, cancelled, userState) {
            this.results = results;
        }
        
        /// <remarks/>
        public Ret Result {
            get {
                this.RaiseExceptionIfNecessary();
                return ((Ret)(this.results[0]));
            }
        }
    }
}
