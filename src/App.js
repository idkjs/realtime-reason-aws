import API, { graphqlOperation } from '@aws-amplify/api';
import Amplify from '@aws-amplify/core';
import '@aws-amplify/pubsub';
import 'bootstrap/dist/css/bootstrap.min.css';
import { default as React, useEffect, useState } from 'react';
import Re from './App.bs';
import AppWorkingSub from './demos/AppWorkingSub.bs';
import './App.css';
import aws_exports from './aws-exports';
// import Demo from './demos/Demo.bs';
import logo from './logo.svg';
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

function App() {
	const [
		message,
		setMessage
	] = useState('');
	const [
		value,
		setValue
	] = useState('');
	const [
		display,
		setDisplay
	] = useState(false);

	const handleSubmit = (event) => {
		event.preventDefault();
		event.stopPropagation();
		const message = {
			id: '',
			message: value,
			createdAt: ''
		};
		console.log('Message: ' + message);
		const mutation = API.graphql(graphqlOperation(createMessage, message));
		console.log('Mutation: ' + JSON.stringify(mutation.data, null, 2));
	};

	useEffect(() => {
		const subscription = API.graphql(graphqlOperation(onCreateMessage)).subscribe({
			next: (event) => {
				if (event) {
					console.log('Subscription: ' + JSON.stringify(event.value.data, null, 2));
					console.log('EVENT: ' + JSON.stringify(event, null, 2));
					setDisplay(true);
					let message = event.value.data.onCreateMessage.message;
					setMessage(message);
				}
			}
		});

		return () => subscription.unsubscribe();
	}, []);

	const handleChange = (e) => {
		console.log(e.target.value);
		// 	const value = e.target.value;
		setValue(e.target.value);
	};
	// function handleChange(event) {
	// 	console.log(event);
	// 	const value = event.target.value;
	// 	setValue(value);
	// }
	return (
		<div className="App">
			<img src={logo} className="App-logo" alt="logo" />
			<div className="jumbotron jumbotron-fluid p-0">
				<h2 className="center">Broadcaster</h2>
			</div>
			<br />
			<div className="container">
				<form onSubmit={handleSubmit}>
					<div className="form-group">
						<input
							className="form-control form-control-lg"
							type="text"
							value={value}
							onChange={handleChange}
						/>
					</div>
					<input type="submit" value="Submit" className="btn btn-primary" />
				</form>
			</div>
			<br />
			{
				display ? <div className="container">
					<div className="card bg-success">
						<h3 className="card-text text-white p-2">{message}</h3>
					</div>
				</div> :
				null}
			{/* <Re /> */}
			<AppWorkingSub />
		</div>
	);
}
export default App;
