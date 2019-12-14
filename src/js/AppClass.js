import React, { Component } from "react";
import { useEffect, useReducer } from 'react'
import "./App.css";
import "bootstrap/dist/css/bootstrap.min.css";
import logo from "./logo.svg";
import Amplify from "@aws-amplify/core";
import "@aws-amplify/pubsub";
import API, { graphqlOperation } from "@aws-amplify/api";
import aws_exports from "../aws-exports";
Amplify.configure(aws_exports);

const createMessage = `mutation createMessage($message: String!){
    createMessage(input:{message:$message}) {
    __typename
    id
    message
    createdAt
    }
}
`;

const onCreateMessage = `subscription onCreateMessage {
    onCreateMessage {
    __typename
    message
    }
}`;

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      message: "",
      value: "",
      display: false
    };
    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  async componentDidMount() {
    this.subscription = API.graphql(
      graphqlOperation(onCreateMessage)
    ).subscribe({
      next: event => {
        if (event){
          console.log("Subscription: " + JSON.stringify(event.value.data, null, 2));
          console.log("EVENT: " + JSON.stringify(event, null, 2));
          this.setState({ display: true });
          this.setState({ message: event.value.data.onCreateMessage.message });
        }
      }
    });
  }

  handleChange(event) {
    console.log(event)
    this.setState({ value: event.target.value });
  }

  async handleSubmit(event) {
    event.preventDefault();
    event.stopPropagation();
    const message = {
      id: "",
      message: this.state.value,
      createdAt: ""
    };
    const mutation = await API.graphql(
      graphqlOperation(createMessage, message)
    );
    console.log("Mutation: " + JSON.stringify(mutation.data, null, 2));
  }

  render() {
    return (
      <div className="App">
        <img src={logo} className="App-logo" alt="logo" />
        <div className="jumbotron jumbotron-fluid p-0">
          <h2 className="center">Broadcaster</h2>
        </div>
        <br />
        <div className="container">
          <form onSubmit={this.handleSubmit}>
            <div className="form-group">
              <input
                className="form-control form-control-lg"
                type="text"
                value={this.state.value}
                onChange={this.handleChange}
              />
            </div>
            <input
              type="submit"
              value="Submit"
              className="btn btn-primary"
            />
          </form>
        </div>
        <br />
        {this.state.display ? (
          <div className="container">
            <div className="card bg-success">
              <h3 className="card-text text-white p-2">
                {this.state.message}
              </h3>
            </div>
          </div>
        ) : null}
      </div>
    );
  }
}

export default App;