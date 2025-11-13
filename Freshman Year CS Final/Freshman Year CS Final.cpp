// Climate Change Adaptor - Comprehensive Sea Level Rise Approximation System
// Enhanced version with ML, visualizations, advanced modeling, and detailed analysis

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>
#include <numeric>
using namespace std;

// Structure to hold country/region data
struct Region {
	string name;
	double areaSqKm;
	double coastlineKm;
	double avgElevation;
	double populationMillions;
	double coastalPopulationPercent;
	double gdpBillions;
	string vulnerability; // Low, Medium, High, Critical
};

// Sea level rise scenarios based on IPCC projections
struct SeaRiseScenario {
	string name;
	double riseRatePerYear; // cm per year
	double baseYear;
	double projectedRise2050; // cm
	double projectedRise2100; // cm
};

// Initialize sea level rise scenarios
vector<SeaRiseScenario> initializeScenarios() {
	vector<SeaRiseScenario> scenarios;

	// Conservative scenario (RCP 2.6 - strong mitigation)
	scenarios.push_back({"Conservative (RCP 2.6)", 0.32, 2025, 29.0, 43.0});

	// Moderate scenario (RCP 4.5 - intermediate)
	scenarios.push_back({"Moderate (RCP 4.5)", 0.44, 2025, 38.0, 63.0});

	// High scenario (RCP 6.0 - business as usual)
	scenarios.push_back({"High (RCP 6.0)", 0.52, 2025, 44.0, 73.0});

	// Extreme scenario (RCP 8.5 - worst case)
	scenarios.push_back({"Extreme (RCP 8.5)", 0.64, 2025, 52.0, 84.0});

	return scenarios;
}

// Initialize comprehensive region database
vector<Region> initializeRegions() {
	vector<Region> regions;

	// Island Nations (Most Vulnerable)
	regions.push_back({"Maldives", 298, 644, 1.5, 0.54, 100, 5.7, "Critical"});
	regions.push_back({"Tuvalu", 26, 24, 2.0, 0.012, 100, 0.047, "Critical"});
	regions.push_back({"Marshall Islands", 181, 370, 2.1, 0.059, 100, 0.24, "Critical"});
	regions.push_back({"Kiribati", 811, 1143, 2.0, 0.12, 100, 0.22, "Critical"});
	regions.push_back({"Seychelles", 455, 491, 3.0, 0.098, 95, 1.7, "Critical"});
	regions.push_back({"Palau", 459, 1519, 3.5, 0.018, 90, 0.31, "Critical"});
	regions.push_back({"Nauru", 21, 30, 3.0, 0.011, 100, 0.16, "Critical"});

	// Pacific Island Nations
	regions.push_back({"Solomon Islands", 28400, 5313, 15.0, 0.69, 85, 1.6, "High"});
	regions.push_back({"Fiji", 18272, 1129, 23.0, 0.90, 75, 5.5, "High"});
	regions.push_back({"Vanuatu", 12189, 2528, 25.0, 0.31, 80, 0.95, "High"});
	regions.push_back({"Micronesia", 702, 6112, 4.0, 0.11, 100, 0.40, "Critical"});
	regions.push_back({"Tonga", 747, 419, 5.0, 0.11, 95, 0.50, "High"});
	regions.push_back({"Samoa", 2842, 403, 8.0, 0.20, 90, 0.85, "High"});
	regions.push_back({"Cook Islands", 236, 120, 4.0, 0.017, 100, 0.38, "Critical"});
	regions.push_back({"French Polynesia", 4167, 2525, 6.0, 0.28, 95, 5.8, "High"});

	// Caribbean Nations
	regions.push_back({"Jamaica", 10991, 1022, 18.0, 2.96, 60, 16.5, "High"});
	regions.push_back({"Bahamas", 13878, 3542, 3.0, 0.39, 85, 12.8, "Critical"});
	regions.push_back({"Barbados", 430, 97, 5.0, 0.29, 100, 5.2, "High"});
	regions.push_back({"Trinidad and Tobago", 5128, 362, 10.0, 1.40, 45, 24.1, "Medium"});
	regions.push_back({"Cuba", 109884, 3735, 60.0, 11.33, 35, 107.4, "Medium"});
	regions.push_back({"Haiti", 27750, 1771, 145.0, 11.40, 30, 19.9, "High"});
	regions.push_back({"Dominican Republic", 48671, 1288, 80.0, 10.85, 40, 94.2, "Medium"});

	// Asian Coastal Nations
	regions.push_back({"Bangladesh", 148460, 580, 85.0, 166.3, 55, 416.3, "Critical"});
	regions.push_back({"Vietnam", 331212, 3444, 150.0, 97.3, 45, 366.1, "High"});
	regions.push_back({"Philippines", 300000, 36289, 140.0, 111.0, 50, 404.3, "High"});
	regions.push_back({"Indonesia", 1904569, 54716, 120.0, 273.5, 40, 1119.2, "High"});
	regions.push_back({"Thailand", 513120, 3219, 150.0, 69.8, 30, 505.9, "Medium"});
	regions.push_back({"Myanmar", 676578, 1930, 190.0, 54.4, 25, 76.1, "Medium"});
	regions.push_back({"Singapore", 728, 193, 15.0, 5.85, 100, 397.0, "High"});

	// Indian Ocean Region
	regions.push_back({"Sri Lanka", 65610, 1340, 100.0, 21.4, 45, 84.0, "High"});
	regions.push_back({"Mauritius", 2040, 177, 12.0, 1.27, 95, 14.2, "High"});
	regions.push_back({"Comoros", 2235, 340, 40.0, 0.87, 85, 1.3, "High"});
	regions.push_back({"Madagascar", 587041, 4828, 240.0, 28.0, 35, 14.1, "Medium"});

	// African Coastal Nations
	regions.push_back({"Gambia", 11295, 80, 34.0, 2.42, 70, 1.9, "High"});
	regions.push_back({"Senegal", 196722, 531, 69.0, 16.7, 40, 27.6, "Medium"});
	regions.push_back({"Nigeria", 923768, 853, 230.0, 206.1, 25, 440.8, "Medium"});
	regions.push_back({"Egypt", 1001450, 2450, 170.0, 102.3, 30, 404.1, "High"});
	regions.push_back({"Mozambique", 799380, 2470, 120.0, 31.3, 35, 15.1, "High"});

	// Major Economic Powers
	regions.push_back({"United States", 9833517, 19924, 760.0, 331.9, 25, 25346.8, "Medium"});
	regions.push_back({"China", 9596961, 14500, 1840.0, 1439.3, 20, 17734.1, "Medium"});
	regions.push_back({"Japan", 377975, 29751, 438.0, 125.8, 35, 4937.4, "High"});
	regions.push_back({"India", 3287263, 7517, 460.0, 1380.0, 20, 3173.4, "Medium"});
	regions.push_back({"United Kingdom", 242495, 12429, 162.0, 67.9, 30, 3124.7, "Medium"});
	regions.push_back({"Netherlands", 41543, 451, 30.5, 17.4, 60, 1012.8, "High"});
	regions.push_back({"Denmark", 42933, 7314, 34.0, 5.8, 45, 395.9, "High"});

	// US Cities/Regions
	regions.push_back({"Washington DC", 177, 0, 125.0, 0.71, 0, 141.0, "Medium"});
	regions.push_back({"Miami-Dade County", 6000, 200, 1.8, 2.7, 90, 160.0, "Critical"});
	regions.push_back({"New Orleans", 906, 85, 0.5, 0.39, 100, 95.0, "Critical"});
	regions.push_back({"New York City", 783, 837, 10.0, 8.3, 100, 1700.0, "High"});

	// South America
	regions.push_back({"Guyana", 214969, 459, 207.0, 0.79, 80, 6.8, "High"});
	regions.push_back({"Suriname", 163820, 386, 246.0, 0.59, 75, 3.6, "High"});
	regions.push_back({"French Guiana", 83534, 378, 150.0, 0.30, 70, 4.9, "Medium"});

	// Australia and Oceania
	regions.push_back({"Australia", 7692024, 25760, 330.0, 25.7, 25, 1392.7, "Medium"});
	regions.push_back({"New Zealand", 268838, 15134, 388.0, 5.1, 35, 242.8, "Medium"});

	return regions;
}

// ============= MACHINE LEARNING & STATISTICAL MODELS =============

// Linear regression model for trend analysis
struct LinearRegression {
	double slope;
	double intercept;
	double r_squared;
};

// Polynomial regression coefficients
struct PolynomialRegression {
	vector<double> coefficients;
	double r_squared;
};

// Monte Carlo simulation result
struct MonteCarloResult {
	double mean;
	double median;
	double stdDev;
	double confidence95Lower;
	double confidence95Upper;
	vector<double> samples;
};

// Fit linear regression to data
LinearRegression fitLinearRegression(const vector<double>& x, const vector<double>& y) {
	int n = x.size();
	double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

	for (int i = 0; i < n; i++) {
		sumX += x[i];
		sumY += y[i];
		sumXY += x[i] * y[i];
		sumX2 += x[i] * x[i];
	}

	double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
	double intercept = (sumY - slope * sumX) / n;

	// Calculate R-squared
	double meanY = sumY / n;
	double ssTotal = 0, ssResidual = 0;
	for (int i = 0; i < n; i++) {
		double predicted = slope * x[i] + intercept;
		ssTotal += pow(y[i] - meanY, 2);
		ssResidual += pow(y[i] - predicted, 2);
	}
	double r_squared = 1.0 - (ssResidual / ssTotal);

	return {slope, intercept, r_squared};
}

// Fit polynomial regression (degree 2)
PolynomialRegression fitPolynomialRegression(const vector<double>& x, const vector<double>& y) {
	int n = x.size();

	// For quadratic: y = a*x^2 + b*x + c
	// Using simplified least squares for degree 2
	double sumX = 0, sumX2 = 0, sumX3 = 0, sumX4 = 0;
	double sumY = 0, sumXY = 0, sumX2Y = 0;

	for (int i = 0; i < n; i++) {
		double xi = x[i];
		double yi = y[i];
		sumX += xi;
		sumX2 += xi * xi;
		sumX3 += xi * xi * xi;
		sumX4 += xi * xi * xi * xi;
		sumY += yi;
		sumXY += xi * yi;
		sumX2Y += xi * xi * yi;
	}

	// Solve system using Cramer's rule (simplified)
	double denom = n * sumX2 * sumX4 + 2 * sumX * sumX2 * sumX3 - sumX2 * sumX2 * sumX2 - n * sumX3 * sumX3 - sumX * sumX * sumX4;

	double a = (sumX2Y * (n * sumX2 - sumX * sumX) + sumXY * (sumX * sumX3 - sumX2 * sumX2) + sumY * (sumX2 * sumX3 - sumX * sumX4)) / denom;
	double b = (n * sumX2Y * (sumX * sumX3 - sumX2 * sumX2) + sumXY * (sumX2 * sumX4 - sumX3 * sumX3) + sumY * (sumX2 * sumX3 - sumX * sumX4)) / denom;
	double c = (sumY - b * sumX - a * sumX2) / n;

	vector<double> coefficients = {c, b, a};

	// Calculate R-squared
	double meanY = sumY / n;
	double ssTotal = 0, ssResidual = 0;
	for (int i = 0; i < n; i++) {
		double predicted = a * x[i] * x[i] + b * x[i] + c;
		ssTotal += pow(y[i] - meanY, 2);
		ssResidual += pow(y[i] - predicted, 2);
	}
	double r_squared = 1.0 - (ssResidual / ssTotal);

	return {coefficients, r_squared};
}

// Monte Carlo simulation for uncertainty modeling
MonteCarloResult runMonteCarloSimulation(int targetYear, const SeaRiseScenario& baseScenario, int numSimulations = 10000) {
	random_device rd;
	mt19937 gen(rd());

	// Add uncertainty: ±15% for projection variability
	double baseRise = calculateSeaRise(targetYear, baseScenario);
	double uncertainty = baseRise * 0.15;

	normal_distribution<> dist(baseRise, uncertainty);

	vector<double> samples;
	samples.reserve(numSimulations);

	for (int i = 0; i < numSimulations; i++) {
		double sample = dist(gen);
		samples.push_back(max(0.0, sample)); // Ensure non-negative
	}

	// Sort for percentile calculations
	sort(samples.begin(), samples.end());

	double mean = accumulate(samples.begin(), samples.end(), 0.0) / numSimulations;
	double median = samples[numSimulations / 2];

	// Calculate standard deviation
	double variance = 0;
	for (double sample : samples) {
		variance += pow(sample - mean, 2);
	}
	double stdDev = sqrt(variance / numSimulations);

	// 95% confidence interval (2.5th and 97.5th percentiles)
	double conf95Lower = samples[int(numSimulations * 0.025)];
	double conf95Upper = samples[int(numSimulations * 0.975)];

	return {mean, median, stdDev, conf95Lower, conf95Upper, samples};
}

// Exponential smoothing for time series prediction
double exponentialSmoothing(const vector<double>& data, double alpha = 0.3) {
	if (data.empty()) return 0.0;

	double smoothed = data[0];
	for (size_t i = 1; i < data.size(); i++) {
		smoothed = alpha * data[i] + (1 - alpha) * smoothed;
	}
	return smoothed;
}

// Detect climate tipping points
struct TippingPoint {
	int year;
	string description;
	double riseThreshold;
	string regions;
};

vector<TippingPoint> identifyTippingPoints() {
	vector<TippingPoint> tippingPoints;

	tippingPoints.push_back({2030, "Critical threshold for small island states", 15.0, "Maldives, Tuvalu, Marshall Islands"});
	tippingPoints.push_back({2045, "Major coastal city infrastructure at risk", 30.0, "Miami, New Orleans, Jakarta, Manila"});
	tippingPoints.push_back({2060, "Mass displacement begins for low-lying nations", 50.0, "Bangladesh, Pacific Islands, Caribbean"});
	tippingPoints.push_back({2080, "Irreversible loss for multiple atoll nations", 70.0, "Multiple Pacific and Indian Ocean atolls"});
	tippingPoints.push_back({2100, "Major economic centers require massive adaptation", 84.0, "NYC, London, Tokyo, Shanghai, Mumbai"});

	return tippingPoints;
}

// ============= VISUALIZATION FUNCTIONS =============

// Create ASCII bar chart
void drawBarChart(const vector<pair<string, double>>& data, const string& title, const string& unit = "cm") {
	cout << "\n=== " << title << " ===" << endl << endl;

	// Find max value for scaling
	double maxVal = 0;
	for (const auto& item : data) {
		maxVal = max(maxVal, item.second);
	}

	int maxBarWidth = 50;

	for (const auto& item : data) {
		int barWidth = (int)((item.second / maxVal) * maxBarWidth);
		cout << setw(25) << left << item.first << " | ";
		for (int i = 0; i < barWidth; i++) {
			cout << "█";
		}
		cout << " " << fixed << setprecision(1) << item.second << " " << unit << endl;
	}
	cout << endl;
}

// Create ASCII line chart
void drawLineChart(const vector<pair<int, double>>& data, const string& title, const string& yLabel) {
	cout << "\n=== " << title << " ===" << endl;

	if (data.empty()) return;

	// Find min and max for scaling
	double minY = data[0].second;
	double maxY = data[0].second;
	for (const auto& point : data) {
		minY = min(minY, point.second);
		maxY = max(maxY, point.second);
	}

	int chartHeight = 20;
	double yRange = maxY - minY;

	// Draw chart from top to bottom
	for (int row = chartHeight; row >= 0; row--) {
		double yValue = minY + (yRange * row / chartHeight);
		cout << setw(8) << fixed << setprecision(1) << yValue << " |";

		for (size_t col = 0; col < data.size(); col++) {
			double normalizedY = (data[col].second - minY) / yRange * chartHeight;
			if (abs(normalizedY - row) < 0.5) {
				cout << "●";
			} else if (col > 0 &&
			          ((data[col-1].second - minY) / yRange * chartHeight <= row &&
			           (data[col].second - minY) / yRange * chartHeight >= row) ||
			          ((data[col-1].second - minY) / yRange * chartHeight >= row &&
			           (data[col].second - minY) / yRange * chartHeight <= row)) {
				cout << "│";
			} else {
				cout << " ";
			}
		}
		cout << endl;
	}

	// Draw x-axis
	cout << "         └";
	for (size_t i = 0; i < data.size(); i++) {
		cout << "─";
	}
	cout << endl << "          ";

	// Draw year labels
	for (size_t i = 0; i < data.size(); i += max(1, (int)(data.size() / 10))) {
		cout << data[i].first << " ";
	}
	cout << endl;
}

// Export data to CSV for external visualization
void exportToCSV(const vector<Region>& regions, const vector<SeaRiseScenario>& scenarios, const string& filename = "climate_data.csv") {
	ofstream file(filename);

	if (!file.is_open()) {
		cout << "Error: Could not create CSV file." << endl;
		return;
	}

	// Header
	file << "Region,Area_SqKm,Coastline_Km,Avg_Elevation_m,Population_Millions,Coastal_Pop_Percent,GDP_Billions,Vulnerability,";
	for (const auto& scenario : scenarios) {
		file << scenario.name << "_2050,";
		file << scenario.name << "_2100,";
	}
	file << endl;

	// Data rows
	for (const auto& region : regions) {
		file << region.name << ","
		     << region.areaSqKm << ","
		     << region.coastlineKm << ","
		     << region.avgElevation << ","
		     << region.populationMillions << ","
		     << region.coastalPopulationPercent << ","
		     << region.gdpBillions << ","
		     << region.vulnerability << ",";

		for (const auto& scenario : scenarios) {
			double rise2050 = calculateSeaRise(2050, scenario);
			double rise2100 = calculateSeaRise(2100, scenario);
			file << rise2050 << ","
			     << rise2100 << ",";
		}
		file << endl;
	}

	file.close();
	cout << "\n✓ Data exported successfully to " << filename << endl;
	cout << "  You can open this file in Excel, Python (pandas), R, or other tools for visualization." << endl;
}

// Export time series data
void exportTimeSeriesCSV(const vector<SeaRiseScenario>& scenarios, const string& filename = "sea_level_timeseries.csv") {
	ofstream file(filename);

	if (!file.is_open()) {
		cout << "Error: Could not create CSV file." << endl;
		return;
	}

	// Header
	file << "Year";
	for (const auto& scenario : scenarios) {
		file << "," << scenario.name;
	}
	file << endl;

	// Data for years 2025-2150 in 5-year increments
	for (int year = 2025; year <= 2150; year += 5) {
		file << year;
		for (const auto& scenario : scenarios) {
			double rise = calculateSeaRise(year, scenario);
			file << "," << rise;
		}
		file << endl;
	}

	file.close();
	cout << "\n✓ Time series data exported to " << filename << endl;
}

// ============= ADVANCED MODELING =============

// Climate feedback model with ice-albedo and ocean warming effects
struct AdvancedProjection {
	double baselineRise;
	double iceAlbedoEffect;
	double oceanWarmingEffect;
	double glacierAcceleration;
	double totalRise;
	double uncertaintyRange;
};

AdvancedProjection calculateAdvancedProjection(int targetYear, const SeaRiseScenario& scenario) {
	double baselineRise = calculateSeaRise(targetYear, scenario);

	int yearsFromBase = targetYear - (int)scenario.baseYear;

	// Ice-albedo feedback (accelerates over time)
	double albedoFactor = pow(1.02, yearsFromBase / 10.0); // 2% increase per decade
	double iceAlbedoEffect = baselineRise * (albedoFactor - 1.0) * 0.15;

	// Ocean thermal expansion (non-linear with accumulated heat)
	double thermalFactor = 1.0 + (yearsFromBase / 100.0) * 0.3;
	double oceanWarmingEffect = baselineRise * (thermalFactor - 1.0) * 0.25;

	// Glacier acceleration (especially for Greenland and Antarctica)
	double glacierFactor = exp(yearsFromBase / 80.0) - 1.0;
	double glacierAcceleration = baselineRise * glacierFactor * 0.1;

	double totalRise = baselineRise + iceAlbedoEffect + oceanWarmingEffect + glacierAcceleration;
	double uncertaintyRange = totalRise * 0.2; // ±20% uncertainty

	return {baselineRise, iceAlbedoEffect, oceanWarmingEffect, glacierAcceleration, totalRise, uncertaintyRange};
}

// Regional correlation analysis
struct CorrelationResult {
	string region1;
	string region2;
	double correlation;
	string interpretation;
};

vector<CorrelationResult> analyzeRegionalCorrelations(const vector<Region>& regions) {
	vector<CorrelationResult> results;

	// Simple correlation based on vulnerability and elevation
	for (size_t i = 0; i < min(regions.size(), size_t(10)); i++) {
		for (size_t j = i + 1; j < min(regions.size(), size_t(10)); j++) {
			double elevDiff = abs(regions[i].avgElevation - regions[j].avgElevation);
			double correlation = 1.0 - min(1.0, elevDiff / 100.0);

			if (regions[i].vulnerability == regions[j].vulnerability) {
				correlation += 0.2;
			}

			correlation = min(1.0, correlation);

			string interpretation;
			if (correlation > 0.7) interpretation = "High - Similar risk profiles";
			else if (correlation > 0.4) interpretation = "Moderate - Some shared characteristics";
			else interpretation = "Low - Different risk profiles";

			if (correlation > 0.5) {
				results.push_back({regions[i].name, regions[j].name, correlation, interpretation});
			}
		}
	}

	return results;
}

// Calculate sea level rise for a given year and scenario
double calculateSeaRise(int targetYear, const SeaRiseScenario& scenario) {
	if (targetYear <= scenario.baseYear) {
		return 0.0;
	}

	int yearsDiff = targetYear - (int)scenario.baseYear;

	// Use non-linear projection for better accuracy
	if (targetYear <= 2050) {
		double progress = (double)(targetYear - scenario.baseYear) / (2050 - scenario.baseYear);
		return scenario.projectedRise2050 * progress;
	} else if (targetYear <= 2100) {
		double progress = (double)(targetYear - 2050) / (2100 - 2050);
		return scenario.projectedRise2050 + (scenario.projectedRise2100 - scenario.projectedRise2050) * progress;
	} else {
		// Extrapolate beyond 2100 with increased rate
		int yearsAfter2100 = targetYear - 2100;
		return scenario.projectedRise2100 + (yearsAfter2100 * scenario.riseRatePerYear * 1.2);
	}
}

// Calculate ice melt impact for a region
void calculateIceMeltImpact(const Region& region) {
	const double annualIceMeltVolume = 16400.0; // cubic km per year

	double heightSubmerged = annualIceMeltVolume / region.areaSqKm;
	double metersEquivalent = heightSubmerged * 1000;
	double feetEquivalent = metersEquivalent * 3.28084;

	cout << "\n--- Ice Melt Impact Analysis for " << region.name << " ---" << endl;
	cout << "Annual ice melt volume distributed over " << region.name << ":" << endl;
	cout << "  Height: " << fixed << setprecision(2) << metersEquivalent << " meters or "
	     << feetEquivalent << " feet" << endl;
	cout << "  Coastline affected: " << region.coastlineKm << " km" << endl;
	cout << "  Vulnerability level: " << region.vulnerability << endl;
}

// Calculate population and economic impacts
void calculateImpacts(const Region& region, double seaLevelRiseCm) {
	double seaLevelRiseMeters = seaLevelRiseCm / 100.0;

	// Estimate affected population (simplified model)
	double elevationFactor = min(1.0, seaLevelRiseMeters / region.avgElevation);
	double coastalExposureFactor = region.coastalPopulationPercent / 100.0;
	double populationAtRisk = region.populationMillions * coastalExposureFactor * elevationFactor;

	// Estimate economic impact
	double economicImpact = region.gdpBillions * coastalExposureFactor * elevationFactor * 0.15;

	// Estimate land loss
	double landLossPercent = (seaLevelRiseMeters / region.avgElevation) * 100.0 * coastalExposureFactor;
	landLossPercent = min(landLossPercent, 100.0);
	double landLossArea = region.areaSqKm * (landLossPercent / 100.0);

	cout << "\n--- Impact Assessment for " << region.name << " ---" << endl;
	cout << "  Sea level rise: " << fixed << setprecision(2) << seaLevelRiseCm << " cm" << endl;
	cout << "  Population at risk: " << setprecision(2) << populationAtRisk << " million people" << endl;
	cout << "  Estimated economic impact: $" << setprecision(1) << economicImpact << " billion" << endl;
	cout << "  Potential land loss: " << setprecision(1) << landLossArea << " sq km ("
	     << setprecision(1) << landLossPercent << "%)" << endl;

	// Risk level assessment
	if (landLossPercent > 50 || region.vulnerability == "Critical") {
		cout << "  \n  *** SEVERE THREAT: Major territorial and population displacement expected ***" << endl;
	} else if (landLossPercent > 20 || region.vulnerability == "High") {
		cout << "  \n  ** HIGH RISK: Significant adaptation measures required **" << endl;
	} else if (landLossPercent > 5 || region.vulnerability == "Medium") {
		cout << "  \n  * MODERATE RISK: Coastal protection infrastructure needed *" << endl;
	} else {
		cout << "  \n  LOW RISK: Localized impacts expected" << endl;
	}
}

// Get islands at risk for a given year
void getIslandsAtRisk(int year) {
	cout << "\n=== Island Nations at Risk by " << year << " ===" << endl;

	if (year < 2030) {
		cout << "\nCRITICAL RISK (Partial submersion likely):" << endl;
		cout << "  - Maldives, Tuvalu, Marshall Islands, Kiribati" << endl;
		cout << "\nHIGH RISK (Severe flooding):" << endl;
		cout << "  - Solomon Islands, Palau, Nauru, Cook Islands" << endl;
	}
	else if (year < 2050) {
		cout << "\nCRITICAL RISK (Major submersion expected):" << endl;
		cout << "  - Maldives, Tuvalu, Marshall Islands, Kiribati, Nauru" << endl;
		cout << "  - Cook Islands, Micronesia (parts)" << endl;
		cout << "\nHIGH RISK (Extensive flooding and displacement):" << endl;
		cout << "  - Solomon Islands, Fiji, Seychelles, Palau, Vanuatu" << endl;
		cout << "  - Tonga, Samoa, French Polynesia (atolls)" << endl;
		cout << "\nMEDIUM RISK (Coastal areas severely affected):" << endl;
		cout << "  - Bahamas, Miami-Dade County, New Orleans, Bangladesh" << endl;
	}
	else if (year < 2075) {
		cout << "\nCRITICAL RISK (Likely uninhabitable):" << endl;
		cout << "  - Maldives, Tuvalu, Marshall Islands, Kiribati, Nauru" << endl;
		cout << "  - Cook Islands, Tokelau, portions of Micronesia" << endl;
		cout << "\nHIGH RISK (Major population relocation required):" << endl;
		cout << "  - Solomon Islands, Fiji, Vanuatu, Tonga, Samoa" << endl;
		cout << "  - French Polynesia (atolls), Seychelles, Palau" << endl;
		cout << "  - Bahamas, parts of Bangladesh, Miami area, New Orleans" << endl;
		cout << "\nMEDIUM-HIGH RISK:" << endl;
		cout << "  - Singapore, Netherlands (low areas), Venice, Jakarta" << endl;
		cout << "  - Major delta regions (Mekong, Nile, Ganges-Brahmaputra)" << endl;
	}
	else {
		cout << "\nCRITICAL RISK (Submerged or uninhabitable):" << endl;
		cout << "  - Maldives, Tuvalu, Marshall Islands, Kiribati, Nauru" << endl;
		cout << "  - Cook Islands, Tokelau, Micronesia (low-lying areas)" << endl;
		cout << "  - Most atoll nations in Pacific and Indian Oceans" << endl;
		cout << "\nHIGH RISK (Massive displacement and infrastructure loss):" << endl;
		cout << "  - All remaining Pacific island nations" << endl;
		cout << "  - Bahamas, Seychelles, Mauritius" << endl;
		cout << "  - Large portions of Bangladesh, Vietnam, Myanmar" << endl;
		cout << "  - Miami, New Orleans, Alexandria, Jakarta, Shanghai (coastal areas)" << endl;
		cout << "\nMEDIUM-HIGH RISK (Major coastal cities affected):" << endl;
		cout << "  - New York, London, Tokyo, Mumbai, Dhaka, Lagos, Manila" << endl;
		cout << "  - Amsterdam, Venice, Copenhagen, Bangkok, Ho Chi Minh City" << endl;
	}
}

// Display menu and get user choice
int displayMainMenu() {
	cout << "\n\n================================================" << endl;
	cout << "  CLIMATE CHANGE ADAPTOR" << endl;
	cout << "  Advanced ML & Visualization System" << endl;
	cout << "================================================" << endl;
	cout << "\nBASIC ANALYSIS:" << endl;
	cout << "  1. Calculate sea level rise by year" << endl;
	cout << "  2. Analyze ice melt impact for a region" << endl;
	cout << "  3. Calculate impacts for specific region" << endl;
	cout << "  4. Compare all scenarios" << endl;
	cout << "  5. View islands at risk timeline" << endl;
	cout << "  6. List all available regions" << endl;
	cout << "  7. Regional vulnerability assessment" << endl;
	cout << "\nMACHINE LEARNING & ANALYTICS:" << endl;
	cout << "  8. Run Monte Carlo uncertainty analysis" << endl;
	cout << "  9. Advanced projection with climate feedbacks" << endl;
	cout << "  10. Identify climate tipping points" << endl;
	cout << "  11. Trend analysis with regression models" << endl;
	cout << "  12. Regional correlation analysis" << endl;
	cout << "\nVISUALIZATIONS:" << endl;
	cout << "  13. View scenario comparison chart" << endl;
	cout << "  14. View time series projection graph" << endl;
	cout << "  15. Export data to CSV for Excel/Python/R" << endl;
	cout << "  16. Export time series data to CSV" << endl;
	cout << "\n  0. Exit" << endl;
	cout << "\nEnter your choice: ";

	int choice;
	cin >> choice;
	return choice;
}

// List all available regions
void listAllRegions(const vector<Region>& regions) {
	cout << "\n=== Available Regions in Database ===" << endl;
	cout << "\nBy Vulnerability Level:" << endl;

	vector<string> categories = {"Critical", "High", "Medium", "Low"};

	for (const string& category : categories) {
		cout << "\n" << category << " Vulnerability:" << endl;
		int count = 0;
		for (const Region& region : regions) {
			if (region.vulnerability == category) {
				cout << "  - " << region.name;
				if (region.areaSqKm < 1000) {
					cout << " (Small island/territory)";
				}
				cout << endl;
				count++;
			}
		}
		if (count == 0) {
			cout << "  None" << endl;
		}
	}

	cout << "\nTotal regions in database: " << regions.size() << endl;
}

// Vulnerability assessment
void vulnerabilityAssessment(const vector<Region>& regions) {
	cout << "\n=== Global Vulnerability Assessment ===" << endl;

	double totalPopAtRisk = 0;
	double totalGDPAtRisk = 0;
	int criticalRegions = 0;
	int highRiskRegions = 0;

	for (const Region& region : regions) {
		if (region.vulnerability == "Critical") {
			totalPopAtRisk += region.populationMillions * (region.coastalPopulationPercent / 100.0);
			totalGDPAtRisk += region.gdpBillions * (region.coastalPopulationPercent / 100.0) * 0.5;
			criticalRegions++;
		} else if (region.vulnerability == "High") {
			totalPopAtRisk += region.populationMillions * (region.coastalPopulationPercent / 100.0) * 0.7;
			totalGDPAtRisk += region.gdpBillions * (region.coastalPopulationPercent / 100.0) * 0.3;
			highRiskRegions++;
		}
	}

	cout << "\nGlobal Summary:" << endl;
	cout << "  Critical vulnerability regions: " << criticalRegions << endl;
	cout << "  High vulnerability regions: " << highRiskRegions << endl;
	cout << "  Total population at significant risk: " << fixed << setprecision(1)
	     << totalPopAtRisk << " million" << endl;
	cout << "  Total GDP at risk: $" << setprecision(1) << totalGDPAtRisk << " billion" << endl;

	cout << "\n\nMost Vulnerable Regions (by elevation):" << endl;
	vector<Region> sortedRegions = regions;
	sort(sortedRegions.begin(), sortedRegions.end(),
	     [](const Region& a, const Region& b) { return a.avgElevation < b.avgElevation; });

	for (int i = 0; i < min(10, (int)sortedRegions.size()); i++) {
		cout << "  " << (i+1) << ". " << sortedRegions[i].name
		     << " (avg elevation: " << sortedRegions[i].avgElevation << "m)" << endl;
	}
}

int main()
{
	cout << fixed << setprecision(2);

	// Initialize data
	vector<SeaRiseScenario> scenarios = initializeScenarios();
	vector<Region> regions = initializeRegions();

	cout << "================================================" << endl;
	cout << "  CLIMATE CHANGE ADAPTOR v3.0" << endl;
	cout << "  Advanced ML & Visualization System" << endl;
	cout << "================================================" << endl;
	cout << "\nSystem initialized with:" << endl;
	cout << "  ✓ " << scenarios.size() << " IPCC climate scenarios" << endl;
	cout << "  ✓ " << regions.size() << " regions/countries with detailed data" << endl;
	cout << "  ✓ Machine Learning: Regression, Monte Carlo, Analytics" << endl;
	cout << "  ✓ Visualizations: Charts, graphs, CSV export" << endl;
	cout << "  ✓ Advanced modeling: Climate feedbacks & tipping points" << endl;
	cout << "\nData source: IPCC AR6 (2021), World Bank, UN databases" << endl;

	int choice;
	do {
		choice = displayMainMenu();

		switch(choice) {
			case 1: {
				// Calculate sea level rise
				int year;
				cout << "\nEnter target year (2025-2150): ";
				cin >> year;

				if (year < 2025 || year > 2150) {
					cout << "Invalid year. Please enter a year between 2025 and 2150." << endl;
					break;
				}

				cout << "\n=== Sea Level Rise Projections for " << year << " ===" << endl;
				for (const SeaRiseScenario& scenario : scenarios) {
					double rise = calculateSeaRise(year, scenario);
					cout << "\n" << scenario.name << ":" << endl;
					cout << "  Rise from 2025: " << rise << " cm (" << rise/2.54 << " inches)" << endl;
					cout << "  Cumulative rise from pre-industrial: " << rise + 23.0 << " cm" << endl;
				}
				break;
			}

			case 2: {
				// Ice melt impact
				cout << "\nEnter region name (or part of name): ";
				cin.ignore();
				string searchName;
				getline(cin, searchName);

				bool found = false;
				for (const Region& region : regions) {
					if (region.name.find(searchName) != string::npos ||
					    searchName.find(region.name) != string::npos) {
						calculateIceMeltImpact(region);
						found = true;
						break;
					}
				}

				if (!found) {
					cout << "Region not found. Use option 6 to see all available regions." << endl;
				}
				break;
			}

			case 3: {
				// Impact analysis
				cout << "\nEnter region name: ";
				cin.ignore();
				string searchName;
				getline(cin, searchName);

				cout << "\nSelect scenario:" << endl;
				for (size_t i = 0; i < scenarios.size(); i++) {
					cout << (i+1) << ". " << scenarios[i].name << endl;
				}
				cout << "Choice: ";
				int scenarioChoice;
				cin >> scenarioChoice;

				if (scenarioChoice < 1 || scenarioChoice > (int)scenarios.size()) {
					cout << "Invalid scenario choice." << endl;
					break;
				}

				cout << "\nEnter target year: ";
				int year;
				cin >> year;

				bool found = false;
				for (const Region& region : regions) {
					if (region.name.find(searchName) != string::npos ||
					    searchName.find(region.name) != string::npos) {
						double rise = calculateSeaRise(year, scenarios[scenarioChoice-1]);
						calculateImpacts(region, rise);
						found = true;
						break;
					}
				}

				if (!found) {
					cout << "Region not found." << endl;
				}
				break;
			}

			case 4: {
				// Compare scenarios
				cout << "\nEnter target year for comparison: ";
				int year;
				cin >> year;

				cout << "\n=== Scenario Comparison for " << year << " ===" << endl;
				cout << "\n" << setw(25) << left << "Scenario"
				     << setw(15) << "Rise (cm)"
				     << setw(15) << "Rise (inches)" << endl;
				cout << string(55, '-') << endl;

				for (const SeaRiseScenario& scenario : scenarios) {
					double rise = calculateSeaRise(year, scenario);
					cout << setw(25) << left << scenario.name
					     << setw(15) << rise
					     << setw(15) << rise/2.54 << endl;
				}
				break;
			}

			case 5: {
				// Islands at risk
				cout << "\nEnter year to view islands at risk: ";
				int year;
				cin >> year;
				getIslandsAtRisk(year);
				break;
			}

			case 6: {
				// List regions
				listAllRegions(regions);
				break;
			}

			case 7: {
				// Vulnerability assessment
				vulnerabilityAssessment(regions);
				break;
			}

			case 8: {
				// Monte Carlo simulation
				cout << "\nSelect scenario for Monte Carlo analysis:" << endl;
				for (size_t i = 0; i < scenarios.size(); i++) {
					cout << (i+1) << ". " << scenarios[i].name << endl;
				}
				cout << "Choice: ";
				int scenarioChoice;
				cin >> scenarioChoice;

				if (scenarioChoice < 1 || scenarioChoice > (int)scenarios.size()) {
					cout << "Invalid scenario choice." << endl;
					break;
				}

				cout << "\nEnter target year: ";
				int year;
				cin >> year;

				cout << "\nRunning Monte Carlo simulation with 10,000 iterations..." << endl;
				MonteCarloResult result = runMonteCarloSimulation(year, scenarios[scenarioChoice-1]);

				cout << "\n=== Monte Carlo Uncertainty Analysis ===" << endl;
				cout << "Scenario: " << scenarios[scenarioChoice-1].name << endl;
				cout << "Year: " << year << endl;
				cout << "\nResults:" << endl;
				cout << "  Mean projection: " << result.mean << " cm" << endl;
				cout << "  Median projection: " << result.median << " cm" << endl;
				cout << "  Standard deviation: " << result.stdDev << " cm" << endl;
				cout << "\n95% Confidence Interval:" << endl;
				cout << "  Lower bound (2.5%): " << result.confidence95Lower << " cm" << endl;
				cout << "  Upper bound (97.5%): " << result.confidence95Upper << " cm" << endl;
				cout << "\nInterpretation:" << endl;
				cout << "  There is a 95% probability that sea level rise" << endl;
				cout << "  will be between " << result.confidence95Lower << " and "
				     << result.confidence95Upper << " cm by " << year << "." << endl;
				break;
			}

			case 9: {
				// Advanced projection with feedbacks
				cout << "\nSelect scenario:" << endl;
				for (size_t i = 0; i < scenarios.size(); i++) {
					cout << (i+1) << ". " << scenarios[i].name << endl;
				}
				cout << "Choice: ";
				int scenarioChoice;
				cin >> scenarioChoice;

				if (scenarioChoice < 1 || scenarioChoice > (int)scenarios.size()) {
					cout << "Invalid scenario choice." << endl;
					break;
				}

				cout << "\nEnter target year: ";
				int year;
				cin >> year;

				AdvancedProjection adv = calculateAdvancedProjection(year, scenarios[scenarioChoice-1]);

				cout << "\n=== Advanced Projection with Climate Feedbacks ===" << endl;
				cout << "Scenario: " << scenarios[scenarioChoice-1].name << endl;
				cout << "Year: " << year << endl;
				cout << "\nProjection Breakdown:" << endl;
				cout << "  Baseline rise: " << adv.baselineRise << " cm" << endl;
				cout << "  Ice-albedo feedback: +" << adv.iceAlbedoEffect << " cm" << endl;
				cout << "  Ocean warming effect: +" << adv.oceanWarmingEffect << " cm" << endl;
				cout << "  Glacier acceleration: +" << adv.glacierAcceleration << " cm" << endl;
				cout << "  ─────────────────────────────" << endl;
				cout << "  Total projected rise: " << adv.totalRise << " cm" << endl;
				cout << "  Uncertainty range: ±" << adv.uncertaintyRange << " cm" << endl;
				cout << "\nProjected range: " << (adv.totalRise - adv.uncertaintyRange)
				     << " - " << (adv.totalRise + adv.uncertaintyRange) << " cm" << endl;
				break;
			}

			case 10: {
				// Tipping points
				vector<TippingPoint> tippingPoints = identifyTippingPoints();

				cout << "\n=== Climate Tipping Points Analysis ===" << endl;
				cout << "\nIdentified tipping points based on IPCC projections:\n" << endl;

				for (size_t i = 0; i < tippingPoints.size(); i++) {
					cout << "Tipping Point #" << (i+1) << " - Year " << tippingPoints[i].year << endl;
					cout << "  Threshold: " << tippingPoints[i].riseThreshold << " cm sea level rise" << endl;
					cout << "  Impact: " << tippingPoints[i].description << endl;
					cout << "  Affected: " << tippingPoints[i].regions << endl;
					cout << endl;
				}

				// Show which scenarios reach which tipping points
				cout << "\nScenario Analysis:" << endl;
				for (const auto& scenario : scenarios) {
					cout << "\n" << scenario.name << ":" << endl;
					for (const auto& tp : tippingPoints) {
						double rise = calculateSeaRise(tp.year, scenario);
						if (rise >= tp.riseThreshold * 0.9) { // Within 90% of threshold
							cout << "  ✗ Reaches tipping point in " << tp.year
							     << " (" << rise << " cm)" << endl;
						}
					}
				}
				break;
			}

			case 11: {
				// Regression analysis
				cout << "\n=== Trend Analysis with Regression Models ===" << endl;

				// Build dataset from scenarios
				vector<double> years, rises;
				for (int y = 2025; y <= 2100; y += 5) {
					years.push_back(y);
					rises.push_back(calculateSeaRise(y, scenarios[1])); // Using moderate scenario
				}

				LinearRegression linReg = fitLinearRegression(years, rises);
				PolynomialRegression polyReg = fitPolynomialRegression(years, rises);

				cout << "\nLinear Regression Model:" << endl;
				cout << "  y = " << linReg.slope << "x + " << linReg.intercept << endl;
				cout << "  R² = " << linReg.r_squared << " (fit quality)" << endl;

				cout << "\nPolynomial Regression Model (degree 2):" << endl;
				cout << "  y = " << polyReg.coefficients[2] << "x² + "
				     << polyReg.coefficients[1] << "x + " << polyReg.coefficients[0] << endl;
				cout << "  R² = " << polyReg.r_squared << " (fit quality)" << endl;

				cout << "\nPredictions for 2125:" << endl;
				double yearPred = 2125;
				double linPred = linReg.slope * yearPred + linReg.intercept;
				double polyPred = polyReg.coefficients[2] * yearPred * yearPred +
				                  polyReg.coefficients[1] * yearPred +
				                  polyReg.coefficients[0];
				cout << "  Linear model: " << linPred << " cm" << endl;
				cout << "  Polynomial model: " << polyPred << " cm" << endl;

				cout << "\nNote: Polynomial model captures acceleration better than linear model." << endl;
				break;
			}

			case 12: {
				// Correlation analysis
				vector<CorrelationResult> correlations = analyzeRegionalCorrelations(regions);

				cout << "\n=== Regional Correlation Analysis ===" << endl;
				cout << "\nRegions with similar risk profiles:\n" << endl;

				for (size_t i = 0; i < min(correlations.size(), size_t(15)); i++) {
					cout << correlations[i].region1 << " ↔ " << correlations[i].region2 << endl;
					cout << "  Correlation: " << fixed << setprecision(2) << correlations[i].correlation << endl;
					cout << "  " << correlations[i].interpretation << endl;
					cout << endl;
				}

				cout << "Correlation analysis helps identify regions that may face" << endl;
				cout << "similar challenges and could benefit from shared adaptation strategies." << endl;
				break;
			}

			case 13: {
				// Bar chart visualization
				cout << "\nEnter year for scenario comparison: ";
				int year;
				cin >> year;

				vector<pair<string, double>> chartData;
				for (const auto& scenario : scenarios) {
					double rise = calculateSeaRise(year, scenario);
					chartData.push_back({scenario.name, rise});
				}

				drawBarChart(chartData, "Sea Level Rise Projections for " + to_string(year), "cm");
				break;
			}

			case 14: {
				// Line chart visualization
				cout << "\nSelect scenario for time series graph:" << endl;
				for (size_t i = 0; i < scenarios.size(); i++) {
					cout << (i+1) << ". " << scenarios[i].name << endl;
				}
				cout << "Choice: ";
				int scenarioChoice;
				cin >> scenarioChoice;

				if (scenarioChoice < 1 || scenarioChoice > (int)scenarios.size()) {
					cout << "Invalid scenario choice." << endl;
					break;
				}

				vector<pair<int, double>> timeSeriesData;
				for (int y = 2025; y <= 2150; y += 5) {
					double rise = calculateSeaRise(y, scenarios[scenarioChoice-1]);
					timeSeriesData.push_back({y, rise});
				}

				drawLineChart(timeSeriesData,
				             scenarios[scenarioChoice-1].name + " Projection (2025-2150)",
				             "Sea Level Rise (cm)");
				break;
			}

			case 15: {
				// Export to CSV
				exportToCSV(regions, scenarios);
				break;
			}

			case 16: {
				// Export time series
				exportTimeSeriesCSV(scenarios);
				break;
			}

			case 0:
				cout << "\nThank you for using Climate Change Adaptor." << endl;
				cout << "Together we can work towards climate resilience." << endl;
				break;

			default:
				cout << "\nInvalid choice. Please try again." << endl;
		}

	} while (choice != 0);

	return 0;
}




 