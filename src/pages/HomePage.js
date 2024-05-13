import { BrowserRouter as Router, Route, Switch } from 'react-router-dom';
import { Link } from 'react-router-dom';

function HomePage() {
    return (
      <>
      <h1>Home Page</h1>
      <Link to="/about"><button>About</button></Link>
      </>
    )
    ;
  }
  export default HomePage;